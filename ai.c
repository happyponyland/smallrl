#include <stdlib.h>

#include "ai.h"
#include "combat.h"
#include "player.h"

//static int pathfind(int, int, int, int *, int *);
static void zombie_turn(level_t *, mob_t *);
static void wander(level_t *, mob_t *);

void enemy_turn(level_t * level, mob_t * mob)
{
    switch(mob->type) {
        case mob_zombie:
        case mob_rat:
        case mob_giant_rat:
        case mob_boreworm:
            zombie_turn(level, mob);
            break;
        default:
            wander(level, mob);
            break;
    }

    mob->turn_counter += 1;
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

    if (abs(mob->position.y - player.mob->position.y) <= 1 &&
        abs(mob->position.x - player.mob->position.x) <= 1)
    {
        attack(mob, player.mob);
        return;
    }

    try_move_mob(level, mob, player.mob->position.y == mob->position.y ? 0 : player.mob->position.y > mob->position.y ? 1 : -1, player.mob->position.x == mob->position.x ? 0 : player.mob->position.x > mob->position.x ? 1 : -1);

    return;
}
