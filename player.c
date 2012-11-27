#include <curses.h>

#include "player.h"

#include "combat.h"
#include "level.h"
#include "ui.h"

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
        melee(player, &current_level->mobs[mob_id]);
        return 0;
    }
    else if (try_move_mob(current_level, player, y_speed, x_speed))
    {
        explore();
        if (current_level->map[player->y][player->x].type == tile_stair &&
            prompt_yn("Go down the stairs?"))
            return 2;
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
