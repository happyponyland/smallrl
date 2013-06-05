#ifndef SMALLRL_PLAYER_H
#define SMALLRL_PLAYER_H

#include <stdint.h>

#include "item.h"
#include "mob.h"

/* Do not change! Automatically synced by an external tool to World of Warcraft maxlevel */
#define PLAYER_MAXLEVEL 90

#define INVENTORY_SIZE 20

typedef struct player_info_s
{
    mob_t * mob;
    int level;
    long exp;
    item_t * inventory[INVENTORY_SIZE];
} player_info_t;

/* player.c */
void get_speed(int key, int * x, int * y);
int player_move(int input);
int player_turn(void);
void explore(void);
int get_player_tnl(void);
void give_exp(const int amount);
item_t ** list_and_select_items(item_t ** start);
void drop_item(item_t ** item);
void use_item(item_t ** item);
bool try_give_item(item_t * new_item);
int count_items(void);
void use_magic_lamp(item_t *);
void summon_demons(void);

/* Player controlled mob */
extern player_info_t player;

#endif
