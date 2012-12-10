#include "stdlib.h"

#include "ai.h"
#include "player.h"

static int pathfind(int, int, int, int *, int *);
static void zombie_turn(level_t *, mob_t *);
static void wander(level_t *, mob_t *);

void enemy_turn(level_t * level, mob_t * mob)
{
    switch(mob->type) {
        case mob_zombie:
            zombie_turn(level, mob);
            break;
        default:
            wander(level, mob);
            break;
    }

    mob->turn_counter += 1;
}


static int pathfind(int id, int dest_y, int dest_x, int * y_speed, int * x_speed)
{
    return 0;
}

static void wander(level_t * level, mob_t * mob)
{
    if(mob->turn_counter < 5)
        return;

    mob->turn_counter = 0;

    try_move_mob(level, mob, -1 + rand() % 3, -1 + rand() % 3);
}

static void zombie_turn(level_t * level, mob_t * mob)
{
    if(mob->turn_counter < 2 + rand() % 3)
        return;

    mob->turn_counter = 0;

    try_move_mob(level, mob, player->y == mob->y ? 0 : player->y > mob->y ? 1 : -1, player->x == mob->x ? 0 : player->x > mob->x ? 1 : -1);
}
