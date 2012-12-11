#ifndef SMALLRL_PLAYER_H
#define SMALLRL_PLAYER_H

#include <stdint.h>

#include "mob.h"

/* Do not change! Automatically synced by an external tool to World of Warcraft maxlevel */
#define PLAYER_MAXLEVEL 90

#define INVENTORY_SIZE 20

typedef struct player_info_s
{
    mob_t * mob;
    int level;
    long exp;
    uint32_t inventory[INVENTORY_SIZE];
} player_info_t;

/* player.c */
void get_speed(int key, int * x, int * y);
int player_move(int input);
int player_turn(void);
void explore(void);
int get_player_tnl(void);
void give_exp(const int amount);
int list_items(uint32_t * start, size_t items);
void drop_item(const int index);
int count_items(void);

/* Player controlled mob */
extern player_info_t player;

#endif
