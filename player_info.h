#ifndef SMALLRL_PLAYER_INFO_H
#define SMALLRL_PLAYER_INFO_H

#include "mob.h"
#include "item.h"

#define INVENTORY_SIZE 20

typedef struct player_info_s player_info_t;
struct player_info_s
{
    mob_t * mob;
    int level;
    long exp;
    item_t * inventory[INVENTORY_SIZE];
};

#endif
