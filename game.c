#include "smallrl.h"


int play()
{
    int i;
    int turn;

    explore();

    while (1)
    {
        draw_map();
        draw_stats();

        if (mob[0].hp <= 0)
            return prompt_yn("You died. Play again?");

        turn = player_turn();

        if (turn == 1)
            return 0;
        else if (turn == 2)
            continue; // give player the first turn on a new level

        for (i = 1; i < MOBS; i++)
        {
            enemy_turn(i);
        }
    }

    return 0;
}





void new_game()
{
    //clear_map();
    current_depth = 1;

    boring_level(&mob[0].x, &mob[0].y);

    mob[0].type = mob_player;
    mob[0].hp = 20;

    player = &mob[0];
    return;
}



int move_mob(mob_t * mob_to_move, int y_speed, int x_speed)
{
    int new_y = mob_to_move->y + y_speed;
    int new_x = mob_to_move->x + x_speed;

    if (on_map(new_y, new_x) &&
        !(level.map[new_y][new_x].flags & tile_unpassable))
    {
        mob_to_move->y = new_y;
        mob_to_move->x = new_x;

        return 1;
    }

    return 0;
}



int make_mob(mob_type_t type, int y, int x)
{
    int i;

    for (i = 0; i < MOBS; i++)
    {
        if (mob[i].type == mob_none)
        {
            mob[i].y = y;
            mob[i].x = x;

            mob[i].type = type;

            switch (type)
            {
            case mob_newbie:
                mob[i].hp = 5;
                break;

            default:
                mob[i].hp = 2;
                break;
            }

            return 1;
        }
    }

    return 0;
}



int get_mob(int y, int x)
{
    int i;

    for (i = 0; i < MOBS; i++)
        if (mob[i].type && mob[i].y == y && mob[i].x == x)
            return i;

    return -1;
}



void mob_name(char * s, mob_type_t type)
{
    switch (type)
    {
    case mob_player:
        strcpy(s, "player");
        break;

    case mob_newbie:
        strcpy(s, "newbie");
        break;

    default:
        strcpy(s, "???");
        break;
    }

    return;
}
