#include "smallrl.h"
#include <sys/time.h>

int play()
{
    int i,j;
    int turn;
    long last_gametime;
    long current_gametime;
    int ticks_to_perform;

    last_gametime = get_time();

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

        current_gametime = get_time();

        ticks_to_perform = (current_gametime - last_gametime) / TIMESTEP;

        if(ticks_to_perform == 0)
            continue;

        for (i = 1; i < MOBS; i++)
        {
            mob_t * current_mob = &mob[i];

            if(!(current_mob->flags & mf_active))
                continue;

            if(current_mob->next_think < current_gametime) {
                for(j = 0; j < ticks_to_perform; ++j) {
                    enemy_tick(current_mob);
                }
            }
        }

        last_gametime = current_gametime;
    }

    return 0;
}

/* ms */
long get_time()
{
    struct timeval current_time;

    gettimeofday(&current_time, NULL);

    return current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
}



void new_game()
{
    int i;

    current_depth = 1;

    /* reset all enemies */
    for(i = 1; i < MOBS; ++i) {
        mob[i].flags = mf_none;
        mob[i].type = mob_none;
    }

    player = &mob[0];

    boring_level(&player->x, &player->y);

    player->type = mob_player;
    player->hp = 20;

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
            mob[i].flags = mf_active;
            mob[i].next_think = get_time() + 10 * TIMESTEP;

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
