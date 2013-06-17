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
#include "chaos.h"

#include "los.h"
#include "log.h"

static int experience_to_level(int);
static turn_command_t player_move(game_t *, int);
static turn_command_t process_input_log(game_t *, int);
static turn_command_t process_input_map(game_t *, int);

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

static turn_command_t player_move(game_t * game, int input)
{
    int x_speed = 0, y_speed = 0;
    int mob_id;
    get_speed(input, &x_speed, &y_speed);
    mob_id = get_mob(game->level, game->player.mob->position.y + y_speed, game->player.mob->position.x + x_speed);

    if (mob_id != -1)
    {
        attack(game, game->player.mob, &(game->level->mobs[mob_id]));
        return turn_command_complete;
    }
    else if (try_move_mob(game, game->level, game->player.mob, y_speed, x_speed))
    {
        explore(game->level, game->player.mob);
        explore_map(game->level, game->player.mob->position);

        if (game->level->map[game->player.mob->position.y * game->level->width + game->player.mob->position.x].type == tile_stair &&
            prompt_yn("Go down the stairs?"))
        {
            return turn_command_descend;
        }

        item_t * item = game->level->map[game->player.mob->position.y * game->level->width + game->player.mob->position.x].item;

        if (item != NULL)
        {
            char item_n[100];
            char line[MSGLEN];

            item_name(item_n, item);

            snprintf(line, MSGLEN, "There is %s here.", item_n);

            print_msg(line);
            wait();

            clear_msg();
        }

        draw_map(game->input_type, game->level);
        return turn_command_complete;
    }

    print_msg("You cannot go there.");
    return turn_command_void;
}

turn_command_t player_turn(game_t * game)
{
    int input;

    while (1)
    {
        input = getch();

        if(input == ERR) {
            draw_log(game->input_type, &(game->log), game->level);
            continue;
        }

        clear_msg();

        if(input == '\t')
        {
            if(game->input_type == input_type_log) {
                timeout(-1);
                game->input_type = input_type_map;
            } else {
                timeout(500);
                game->input_type = input_type_log;
            }

            draw_map(game->input_type, game->level);
            draw_stats(&(game->player), game->level);
            draw_log(game->input_type, &(game->log), game->level);

            continue;
        }

        turn_command_t command;

        if(game->input_type == input_type_map) {
            command = process_input_map(game, input);
        } else {
            command = process_input_log(game, input);
            draw_log(game->input_type, &(game->log), game->level);
        }

        if(command == turn_command_void) {
            continue;
        }

        return command;
    }
}

static turn_command_t process_input_log(game_t * game, int input)
{
    int log_input_index = strlen(game->log.log_input) - 1;

    if(input == KEY_BACKSPACE) {
        if(log_input_index < 0) {
            return turn_command_void;
        }

        game->log.log_input[log_input_index] = '\0';
    }

    if((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z') || input == ' ') {
        game->log.log_input[log_input_index + 1] = input;
        game->log.log_input[log_input_index + 2] = '\0';
    }

    if((input == '\n' || input == KEY_ENTER) && strlen(game->log.log_input) > 0) {
        turn_command_t command;

        command = execute_command(game, game->log.log_input);

        add_log(&(game->log), game->log.log_input);

        game->log.log_input[0] = '\0';
        log_input_index = 0;
        return command;
    }

    return turn_command_void;
}

static turn_command_t process_input_map(game_t * game, int input)
{
    int move;
    item_t ** itemsel;

    switch (input)
    {
        case 'Q':
            if (prompt_yn("Do you want to quit?"))
                return turn_command_quit;
            return turn_command_void;

        case 'g':
        case ',':
            ;
            item_t * item = game->level->map[game->player.mob->position.y * game->level->width + game->player.mob->position.x].item;

            if (item == 0)
            {
                print_msg("Nothing here!");
                return turn_command_void;
            }

            char item_n[100];
            char line[MSGLEN];

            item_name(item_n, item);

            if (!try_give_item(&(game->player), item))
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
                game->level->map[game->player.mob->position.y * game->level->width + game->player.mob->position.x].item = 0;
            }

            clear_msg();
            return turn_command_complete;

        case 'd':
        case 'u':
            if (count_items(&(game->player)) == 0)
            {
                print_msg("You have no items.");
                return turn_command_void;
            }

            if (input == 'd')
                print_msg("Drop which item?");
            else if (input == 'u')
                print_msg("Use which item?");

            itemsel = list_and_select_items(&(game->player), game->player.inventory);

            /* restore view */
            clear_msg();
            draw_map(game->input_type, game->level);

            if (itemsel != NULL)
            {
                if (input == 'd')
                    drop_item(&(game->player), game->level, itemsel);
                else if (input == 'u')
                    use_item(game, itemsel);

                return turn_command_complete;
            }
            return turn_command_void;

        case '.':
            /* Rest/wait/meditate */
            return turn_command_complete;


        case ' ':
            //clear_msg();
            return turn_command_void;

        case '+':
            chaos_duel();
            return turn_command_void;

        case KEY_LEFT:
        case KEY_RIGHT:
        case KEY_UP:
        case KEY_DOWN:
            move = player_move(game, input);
            return move;

        default:
            return turn_command_void;
    }
}

/*
  Lights up the area near a mob, marks it as explored.
*/
void explore(level_t * level, mob_t * mob)
{
    int p_y;
    int p_x;

    int y;
    int x;

    int top;
    int bottom;
    int left;
    int right;

    top = level->view.ul_position.y;
    bottom = level->view.ul_position.y + level->view.height;

    left = level->view.ul_position.x;
    right = level->view.ul_position.x + level->view.width;

    p_y = mob->position.y;
    p_x = mob->position.x;

    for (y = top; y < bottom; y += 1)
        for (x = left; x < right; x += 1)
            level->map[y * level->width + x].is_lit = 0;

    for (y = p_y - 1; y <= p_y + 1; y++)
    {
        for (x = p_x - 1; x <= p_x + 1; x++)
        {
            if (on_map(level, y, x))
            {
                level->map[y * level->width + x].is_explored = true;
                level->map[y * level->width + x].is_lit = true;
                level->map[y * level->width + x].is_periphery = true;
            }
        }
    }

    /* Floodfill open permalit rooms */

    if (level->map[p_y * level->width + p_x].flags & tile_permalit)
    {
        bool change;

        do
        {
            change = false;

            for (y = top; y < bottom; y += 1)
            {
                if(y < 1)
                    continue;

                if(y > level->height - 1)
                    break;

                for (x = left; x < right; x += 1)
                {
                    if(x < 1)
                        continue;

                    if(x > level->width - 1)
                        break;

                    if (level->map[y * level->width + x].flags & tile_noflood)
                        continue;

                    if (level->map[y * level->width + x].is_lit == 0 &&
                        (level->map[y * level->width + x].flags & tile_permalit ||
                         level->map[(y - 1) * level->width + (x - 1)].flags & tile_permalit ||
                         level->map[(y - 1) * level->width + (x + 1)].flags & tile_permalit ||
                         level->map[(y + 1) * level->width + (x - 1)].flags & tile_permalit ||
                         level->map[(y + 1) * level->width + (x + 1)].flags & tile_permalit ||
                         level->map[(y - 1) * level->width + x].flags & tile_permalit ||
                         level->map[y * level->width + (x - 1)].flags & tile_permalit ||
                         level->map[(y + 1) * level->width + x].flags & tile_permalit ||
                         level->map[y * level->width + (x + 1)].flags & tile_permalit ))
                    {
                        if (level->map[(y - 1) * level->width + x].is_lit ||
                            level->map[y * level->width + (x - 1)].is_lit ||
                            level->map[(y + 1) * level->width + x].is_lit ||
                            level->map[y * level->width + (x + 1)].is_lit)
                        {
                            level->map[y * level->width + x].is_lit = 1;
                            level->map[y * level->width + x].is_explored = 1;

                            change = true;
                        }

                    }
                }
            }
        } while (change);
    }

    bool match;

    for (y = top; y < bottom; y += 1)
    {
        for (x = left; x < right; x += 1)
        {
            match = false;

            if ((level->map[y * level->width + x].flags & tile_noflood) == 0)
                continue;

            if (y >= 1 && level->map[(y - 1) * level->width + x].is_lit)
                match = true;

            if (x >= 1 && level->map[y * level->width + (x - 1)].is_lit)
                match = true;

            if (y < level->height - 1 && level->map[(y + 1) * level->width + x].is_lit)
                match = true;

            if (x < level->width + 1 && level->map[y * level->width + (x + 1)].is_lit)
                match = true;

            if (y >= 1 &&
                x >= 1 &&
                level->map[(y - 1) * level->width + (x - 1)].is_lit)
                match = true;

            if (y >= 1 &&
                x <= level->width &&
                level->map[(y - 1) * level->width + (x + 1)].is_lit)
                match = true;

            if (y <= level->height &&
                x >= 1 &&
                level->map[(y + 1) * level->width + (x - 1)].is_lit)
                match = true;

            if (y <= level->height &&
                x <= level->width &&
                level->map[(y + 1) * level->width + (x + 1)].is_lit)
                match = true;


            if (match)
            {
                level->map[y * level->width + x].is_periphery = true;
                level->map[y * level->width + x].is_explored = true;
            }
        }
    }

    return;
}

void give_exp(player_info_t * player, const int amount)
{
    char line[MSGLEN];

    if (player->level >= PLAYER_MAXLEVEL)
        return;

    player->exp += amount;

    while (player->exp >= experience_to_level(player->level))
    {
        player->level += 1;

        attron(A_REVERSE | A_BLINK | A_BOLD | COLOR_PAIR(color_green));
        print_msg("You have gained a level!!");
        attrset(0);
        wait();

        snprintf(line, MSGLEN,
                 "You are now level %d.", player->level);
        print_msg(line);
        wait();
        clear_msg();

        player->mob->attr[ATTR_HP] += 10 + 2 * player->level;
        player->mob->attr[ATTR_MINDAM] += 1;
        player->mob->attr[ATTR_ATTACK] += 5;
        player->mob->attr[ATTR_DODGE] += 5;
    }

    return;
}

int get_player_tnl(player_info_t * player)
{
    return experience_to_level(player->level) - player->exp;
}

static int experience_to_level(int level)
{
    return 50 * (level * level) + 100 * level;
}

item_t ** list_and_select_items(player_info_t * player, item_t ** start)
{
    size_t i;
    int row;
    char item_n[100];
    int input;
    size_t sel;

    item_t * current_item;

    row = 0;

    for (i = 0, current_item = *start; current_item != NULL && i < INVENTORY_SIZE; i += 1, current_item = *(start + i))
    {
        row++;

        move(1 + row, 0);

        item_name(item_n, current_item);

        printw("  %c) %s   ",
               'a' + i, item_n);
    }

    refresh();

    do
    {
        input = getch();

        sel = input - 'a';

        if (sel < INVENTORY_SIZE &&
            player->inventory[sel] != NULL)
        {
            return &player->inventory[sel];
        }

        print_msg("You don't have that!!");
        refresh();
    } while (input != ' ');

    return NULL;
}

int count_items(player_info_t * player)
{
    int c;
    int i;

    c = 0;

    for (i = 0; i < INVENTORY_SIZE; i++)
    {
        if (player->inventory[i] != NULL) {
            c += 1;
        }
    }

    return c;
}

void drop_item(player_info_t * player, level_t * level, item_t ** item)
{
    char item_n[100];
    char msg[MSGLEN];

    //TODO: Make sure this item is acutally in the inventory..

    item_name(item_n, *item);

    snprintf(msg, MSGLEN, "Okay, you drop %s.", item_n);
    print_msg(msg);
    wait();
    clear_msg();

    level->map[player->mob->position.y * level->width + player->mob->position.x].item =
        *item;

    *item = NULL;

    return;
}

void use_item(game_t * game, item_t ** item)
{
    char item_n[100];
    char msg[MSGLEN];
    bool should_dispose;

    //TODO: Make sure this item is actually in the inventory..

    item_name(item_n, *item);

    should_dispose = false;

    switch ((*item)->type)
    {
    case item_type_potion:
        switch((item_subtype_potion_t)(*item)->subtype)
        {
            case item_subtype_potion_heal:
                snprintf(msg, MSGLEN, "You drink %s. This stuff is great!", item_n);
                game->player.mob->attr[ATTR_HP] += 20;
                should_dispose = true;
                break;

            default:
                break;
        }
        break;

        case item_type_bibelot:
            switch((*item)->subtype)
            {
                case item_subtype_bibelot_magic_lamp:
                    use_magic_lamp(game, *item);
                    clear_msg();
                    return;

                default:
                    break;
            }
            break;

    default:
        snprintf(msg, MSGLEN, "You do something with %s, but you're not sure what.", item_n);
        break;
    }

    draw_stats(&(game->player), game->level);

    print_msg(msg);
    wait();
    clear_msg();

    if (should_dispose) {
        *item = NULL;
        if(*(item + 1) != NULL)
        {
            for(int i = 1; ; i += 1) {
                item_t * next_item = *(item + i);

                if(next_item == NULL) {
                    break;
                }

                *(item + i - 1) = next_item;
                *(item + i) = NULL;
            }
        }
    }

    return;
}

void use_magic_lamp(game_t * game, item_t * item)
{
    if (rand() % 5 == 0)
    {
        print_msg("Nothing happens.");
        wait();
        return;
    }

    if (item->state == 0)
    {
        print_msg("Smoke begins to emerge from the magic lamp.");
        wait();
    }
    else if (item->state == 1)
    {
        print_msg("The smoke is getting thicker..");
        wait();
    }
    else if (item->state == 2)
    {
        print_msg("The magic lamp starts to glow faintly.");
        wait();
    }
    else if (item->state == 3)
    {
        print_msg("The glow is getting more intense..");
        wait();
    }
    else if (item->state == 4)
    {
        print_msg("You hear a humming noise from the magic lamp.");
        wait();
    }
    else if (item->state == 5)
    {
        print_msg("The magic lamp explodes!");
        wait();
        summon_demons(&(game->player), game->level);
        return;
    }

    item->state += 1;

    return;
}

void summon_demons(player_info_t * player, level_t * level)
{
    int how_many;
    int y;
    int x;

    how_many = 2 + rand() % 4;

    while (how_many--)
    {
        y = player->mob->position.y - 2 + rand() % 5;
        x = player->mob->position.x - 2 + rand() % 5;

        if (on_map(level, y, x) &&
            get_mob(level, y, x) == -1 &&
            !(level->map[y * level->width + x].flags & tile_unpassable))
        {
            try_make_mob(level, mob_demon, y, x);
        }
    }

    return;
}

/*
  Adds new_item to players inventory, returns false if it's full.
*/
bool try_give_item(player_info_t * player, item_t * new_item)
{
    size_t i;

    for (i = 0; i < INVENTORY_SIZE; i += 1)
    {
        if (player->inventory[i] == NULL)
        {
            player->inventory[i] = new_item;
            return true;
        }
    }

    return false;
}
