#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "player.h"

#include "combat.h"
#include "game.h"
#include "level.h"
#include "ui.h"
#include "item.h"

player_info_t player;

static int experience_to_level(int);

void get_speed(int key, int * x, int * y)
{
    switch (key)
    {
    case KEY_LEFT:
        *x = -1;
        break;
    case KEY_RIGHT:
        *x = 1;
        break;
    case KEY_UP:
        *y = -1;
        break;
    case KEY_DOWN:
        *y = 1;
        break;
    default:
        break;
    }
    return;
}


int player_move(int input)
{
    int x_speed = 0, y_speed = 0;
    int mob_id;
    get_speed(input, &x_speed, &y_speed);
    mob_id = get_mob(current_level, player.mob->position.y + y_speed, player.mob->position.x + x_speed);

    if (mob_id != -1)
    {
        attack(player.mob, &current_level->mobs[mob_id]);
        return 0;
    }
    else if (try_move_mob(current_level, player.mob, y_speed, x_speed))
    {
        explore();

        if (current_level->map[player.mob->position.y * current_level->width + player.mob->position.x].type == tile_stair &&
            prompt_yn("Go down the stairs?"))
        {
            return TURN_DESCEND;
        }

        uint32_t item = current_level->map[player.mob->position.y * current_level->width + player.mob->position.x].item;

        if (item)
        {
            char item_n[100];
            char line[MSGLEN];

            item_name(item_n, item);

            snprintf(line, MSGLEN, "There is %s here.", item_n);

            print_msg(line);
            wait();

            clear_msg();
        }

        draw_map(current_level);
        return TURN_COMPLETE;
    }

    print_msg("You cannot go there.");
    return TURN_ABORT;
}


int player_turn(void)
{
    int input, move, itemsel;

    while (1)
    {
        input = getch();

        clear_msg();

        switch (input)
        {
        case 'Q':
            if (prompt_yn("Do you want to quit?"))
                return TURN_QUIT;
            continue;

        case 'g':
        case ',':
            ;
            uint32_t item = current_level->map[player.mob->position.y * current_level->width + player.mob->position.x].item;

            if (item == 0)
            {
                print_msg("Nothing here!");
                continue;
            }

            char item_n[100];
            char line[MSGLEN];

            item_name(item_n, item);

            if (give_item(item) == 666)
            {
                print_msg("You're carrying too much shit already.");
                wait();
            }
            else
            {
                snprintf(line, MSGLEN,
                         "Okay -- you now have %s.", item_n);
                print_msg(line);
                wait();
                current_level->map[player.mob->position.y * current_level->width + player.mob->position.x].item = 0;
            }

            clear_msg();

            return TURN_COMPLETE;


        case 'd':
        case 'u':
            if (count_items() == 0)
            {
                print_msg("You have no items.");
                continue;
            }

            if (input == 'd')
                print_msg("Drop which item?");
            else if (input == 'u')
                print_msg("Use which item?");

            itemsel = list_items(&player.inventory[0], INVENTORY_SIZE);

            /* restore view */
            clear_msg();
            draw_map(current_level);

            if (itemsel != -1)
            {
                if (input == 'd')
                    drop_item(itemsel);
                else if (input == 'u')
                    use_item(itemsel);

                return TURN_COMPLETE;
            }
            continue;

        case '.':
            /* Rest/wait/meditate */
            return TURN_COMPLETE;


        case ' ':
            //clear_msg();
            continue;

        case KEY_LEFT:
        case KEY_RIGHT:
        case KEY_UP:
        case KEY_DOWN:
            move = player_move(input);
            if (move == TURN_ABORT)
                continue;
            else
                return move;

        default:
            continue;
        }
    }
}



/*
  Lights up the area near the player, marks it as explored.
*/
void explore(void)
{
    int p_y;
    int p_x;

    int y;
    int x;

    int top;
    int bottom;
    int left;
    int right;

    top = current_level->view.ul_position.y;
    bottom = current_level->view.ul_position.y + current_level->view.height;

    left = current_level->view.ul_position.x;
    right = current_level->view.ul_position.x + current_level->view.width;

    p_y = player.mob->position.y;
    p_x = player.mob->position.x;

    for (y = top; y < bottom; y += 1)
        for (x = left; x < right; x += 1)
            current_level->map[y * current_level->width + x].is_lit = 0;

    for (y = p_y - 1; y <= p_y + 1; y++)
    {
        for (x = p_x - 1; x <= p_x + 1; x++)
        {
            if (on_map(current_level, y, x))
            {
                current_level->map[y * current_level->width + x].is_explored = 1;
                current_level->map[y * current_level->width + x].is_lit = 1;
            }
        }
    }

    /* Floodfill open permalit rooms */

    if (current_level->map[p_y * current_level->width + p_x].flags & tile_permalit)
    {
        int change;

        do
        {
            change = 0;

            for (y = top; y < bottom; y += 1)
            {
                if(y < 1)
                    continue;

                if(y > current_level->height - 1)
                    break;

                for (x = left; x < right; x += 1)
                {
                    if(x < 1)
                        continue;

                    if(x > current_level->width - 1)
                        break;

                    if (current_level->map[y * current_level->width + x].is_lit == 0 &&
                        (current_level->map[y * current_level->width + x].flags & tile_permalit ||
                         current_level->map[(y - 1) * current_level->width + (x - 1)].flags & tile_permalit ||
                         current_level->map[(y - 1) * current_level->width + (x + 1)].flags & tile_permalit ||
                         current_level->map[(y + 1) * current_level->width + (x - 1)].flags & tile_permalit ||
                         current_level->map[(y + 1) * current_level->width + (x + 1)].flags & tile_permalit ||
                         current_level->map[(y - 1) * current_level->width + x].flags & tile_permalit ||
                         current_level->map[y * current_level->width + (x - 1)].flags & tile_permalit ||
                         current_level->map[(y + 1) * current_level->width + x].flags & tile_permalit ||
                         current_level->map[y * current_level->width + (x + 1)].flags & tile_permalit ))
                    {
                        if (current_level->map[(y - 1) * current_level->width + x].is_lit ||
                            current_level->map[y * current_level->width + (x - 1)].is_lit ||
                            current_level->map[(y + 1) * current_level->width + x].is_lit ||
                            current_level->map[y * current_level->width + (x + 1)].is_lit)
                        {
                            current_level->map[y * current_level->width + x].is_lit = 1;
                            current_level->map[y * current_level->width + x].is_explored = 1;

                            change = 1;
                        }

                    }
                }
            }
        } while (change);
    }

    return;
}



void give_exp(const int amount)
{
    char line[MSGLEN];

    if (player.level >= PLAYER_MAXLEVEL)
        return;

    player.exp += amount;

    while (player.exp >= experience_to_level(player.level))
    {
        player.level += 1;

        attron(A_REVERSE | A_BLINK | A_BOLD | COLOR_PAIR(color_green));
        print_msg("You have gained a level!!");
        attrset(0);
        wait();

        snprintf(line, MSGLEN,
                 "You are now level %d.", player.level);
        print_msg(line);
        wait();
        clear_msg();

        player.mob->attr[ATTR_HP] += 10 + 2 * player.level;
        player.mob->attr[ATTR_MINDAM] += 1;
        player.mob->attr[ATTR_ATTACK] += 5;
        player.mob->attr[ATTR_DODGE] += 5;
    }

    return;
}

int get_player_tnl(void)
{
    return experience_to_level(player.level) - player.exp;
}

static int experience_to_level(int level)
{
    return 50 * (level * level) + 100 * level;
}

int list_items(uint32_t * start, size_t items)
{
    size_t i;
    int row;
    char item_n[100];
    int input;
    int sel;

    row = 0;

    for (i = 0; i < items; i++)
    {
        if (start[i] == item_void)
            continue;

        row++;

        move(1 + row, 0);

        item_name(item_n, start[i]);

        printw("  %c) %s   ",
               'a' + i, item_n);
    }

    refresh();

    do
    {
        input = getch();
        sel = input - 'a';

        if (sel >= 0 &&
            sel < items &&
            player.inventory[sel])
        {
            return sel;
        }

        print_msg("You don't have that!!");
        refresh();
    } while (input != ' ');

    return -1;
}



int count_items()
{
    int c;
    int i;

    c = 0;

    for (i = 0; i < INVENTORY_SIZE; i++)
    {
        if (player.inventory[i] != 0)
            c++;
    }

    return c;
}



void drop_item(const int index)
{
    char item_n[100];
    char msg[MSGLEN];

    item_name(item_n, player.inventory[index]);

    snprintf(msg, MSGLEN, "Okay, you drop %s.", item_n);
    print_msg(msg);
    wait();
    clear_msg();

    current_level->map[player.mob->position.y * current_level->width + player.mob->position.x].item =
        player.inventory[index];

    player.inventory[index] = 0;

    return;
}



void use_item(const int index)
{
    char item_n[100];
    char msg[MSGLEN];
    int dispose;

    item_name(item_n, player.inventory[index]);

    dispose = 0;

    switch (ITEM_TYPE(player.inventory[index]))
    {
    case item_healing_pot:
        snprintf(msg, MSGLEN, "You drink %s. This stuff is great!", item_n);
        player.mob->attr[ATTR_HP] += 20;
        dispose = 1;
        break;

    case item_telephone:
        use_telephone();
        clear_msg();
        return;

    default:
        snprintf(msg, MSGLEN, "You do something with %s, but you're not sure what.", item_n);
        break;
    }

    draw_stats();

    print_msg(msg);
    wait();
    clear_msg();

    if (dispose)
        player.inventory[index] = 0;

    return;
}



void use_telephone()
{
    if (rand() % 5 == 0)
    {
        print_msg("The line is busy.");
        wait();
        return;
    }

    if (player.phone_status == 0)
    {
        print_msg("\"Hello? Who is this?\"");
        wait();
    }
    else if (player.phone_status == 1)
    {
        print_msg("\"You again? What do you want?\"");
        wait();
    }
    else if (player.phone_status == 2)
    {
        print_msg("\"I want you to stop calling me.\"");
        wait();
    }
    else if (player.phone_status == 3)
    {
        print_msg("\"Seriously, you need to stop this.\"");
        wait();
    }
    else if (player.phone_status == 4)
    {
        print_msg("\"If you call me one more time, I will send the cops after you.\"");
        wait();
    }
    else if (player.phone_status == 5)
    {
        print_msg("\"I've had enough of this.\"");
        wait();
        summon_cops();
        return;
    }

    player.phone_status++;

    return;
}


void summon_cops()
{
    int how_many;
    int y;
    int x;

    how_many = 2 + rand() % 4;

    while (how_many--)
    {
        y = player.mob->position.y - 2 + rand() % 5;
        x = player.mob->position.x - 2 + rand() % 5;

        if (on_map(current_level, y, x) &&
            get_mob(current_level, y, x) == -1 &&
            !(current_level->map[y * current_level->width + x].flags & tile_unpassable))
        {
            try_make_mob(current_level, mob_police, y, x);
        }
    }

    return;
}



/*
  Adds new_item to players inventory, returns 666 if it's full.
*/
int give_item(uint32_t new_item)
{
    size_t i;

    for (i = 0; i < INVENTORY_SIZE; i++)
    {
        if (player.inventory[i] == 0)
        {
            player.inventory[i] = new_item;
            return 0;
        }
    }

    return 666;
}
