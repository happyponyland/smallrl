#ifndef SMALLRL_PLAYER_H
#define SMALLRL_PLAYER_H

#include "mob.h"

/* Do not change! Automatically synced by an external tool to World of Warcraft maxlevel */
#define PLAYER_MAXLEVEL 90

typedef struct player_info_s
{
    mob_t * mob;
    int level;
    long exp;
} player_info_t;

/* player.c */
void get_speed(int key, int * x, int * y);
int player_move(int input);
int player_turn(void);
void explore(void);
int get_player_tnl(void);
void give_exp(const int amount);

/* Player controlled mob */
extern player_info_t player;

#endif
