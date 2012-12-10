#include <curses.h>

#include "player.h"

#include "combat.h"
#include "level.h"
#include "ui.h"

int player_level = 1;
long player_exp = 0;
long player_tnl[] =
{
    0,
    100,
    250,
    500,
    900,
    1500,
    2500,
    3800,
    7000,
    12000,
    15000,
    25000,
    60000,
    90000,
    200000,
    350000,
    600000,
    1000000,
    2000000,
    3000000,
    4000000,
    5000000,
    6000000,
    7000000
};

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
    mob_id = get_mob(current_level, player->y + y_speed, player->x + x_speed);

    if (mob_id != -1)
    {
        attack(player, &current_level->mobs[mob_id]);
        return 0;
    }
    else if (try_move_mob(current_level, player, y_speed, x_speed))
    {
        explore();
        if (current_level->map[player->y][player->x].type == tile_stair &&
            prompt_yn("Go down the stairs?"))
        {
            return 2;
        }

        draw_map(current_level);
        return 0;
    }

    print_msg("You cannot go there.");
    return -1;
}


int player_turn(void)
{
    int input, move;

    while (1)
    {
        input = getch();

        clear_msg();

        switch (input)
        {
        case 'Q':
            if (prompt_yn("Do you want to quit?"))
                return 1;
            continue;

        case ' ':
            //clear_msg();
            continue;

        case KEY_LEFT:
        case KEY_RIGHT:
        case KEY_UP:
        case KEY_DOWN:
            move = player_move(input);
            if (move == -1)
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

    p_y = player->y;
    p_x = player->x;

    for (y = 0; y < MAP_H; y++)
        for (x = 0; x < MAP_W; x++)
            current_level->map[y][x].is_lit = 0;

    for (y = p_y - 1; y <= p_y + 1; y++)
    {
        for (x = p_x - 1; x <= p_x + 1; x++)
        {
            if (on_map(current_level, y, x))
            {
                current_level->map[y][x].is_explored = 1;
                current_level->map[y][x].is_lit = 1;
            }
        }
    }

    /* Floodfill open permalit rooms */

    if (current_level->map[p_y][p_x].flags & tile_permalit)
    {
        int change;

        do
        {
            change = 0;

            for (y = 1; y < MAP_H - 1; y++)
            {
                for (x = 1; x < MAP_W - 1; x++)
                {
                    if (current_level->map[y][x].is_lit == 0 &&
                        (current_level->map[y][x].flags & tile_permalit ||
                         current_level->map[y - 1][x - 1].flags & tile_permalit ||
                         current_level->map[y - 1][x + 1].flags & tile_permalit ||
                         current_level->map[y + 1][x - 1].flags & tile_permalit ||
                         current_level->map[y + 1][x + 1].flags & tile_permalit ||
                         current_level->map[y - 1][x].flags & tile_permalit ||
                         current_level->map[y][x - 1].flags & tile_permalit ||
                         current_level->map[y + 1][x].flags & tile_permalit ||
                         current_level->map[y][x + 1].flags & tile_permalit ))
                    {
                        if (current_level->map[y - 1][x].is_lit ||
                            current_level->map[y][x - 1].is_lit ||
                            current_level->map[y + 1][x].is_lit ||
                            current_level->map[y][x + 1].is_lit)
                        {
                            current_level->map[y][x].is_lit = 1;
                            current_level->map[y][x].is_explored = 1;

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

    if (player_level >= PLAYER_MAXLEVEL)
        return;

    player_exp += amount;

    while (player_exp >= player_tnl[player_level])
    {
        player_level++;
        
        attron(A_REVERSE | A_BLINK | A_BOLD | COLOR_PAIR(color_green));
        print_msg("You have gained a level!!");
        attrset(0);
        wait();

        snprintf(line, MSGLEN,
                 "You are now level %d.", player_level);
        print_msg(line);
        wait();
        clear_msg();

        player->attr[ATTR_HP] += 10;
        player->attr[ATTR_MINDAM] += 1;
        player->attr[ATTR_ATTACK] += 5;
        player->attr[ATTR_DODGE] += 5;
    }

    return;
}
