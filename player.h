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
    int magic_lamp_status;
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
void use_item(const int index);
int give_item(uint32_t new_item);
int count_items(void);
void use_magic_lamp(void);
void summon_demons(void);

/* Player controlled mob */
extern player_info_t player;

#endif
