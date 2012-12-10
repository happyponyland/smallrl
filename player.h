#ifndef SMALLRL_PLAYER_H
#define SMALLRL_PLAYER_H

#include "mob.h"

extern int player_level;
extern long player_exp;
extern long player_tnl[];

#define PLAYER_MAXLEVEL 20    

/* player.c */
void get_speed(int key, int * x, int * y);
int player_move(int input);
int player_turn(void);
void explore(void);
void give_exp(const int amount);

/* Pointer to player controlled mob */
mob_t * player;

#endif
