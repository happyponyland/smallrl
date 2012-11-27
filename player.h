#ifndef SMALLRL_PLAYER_H
#define SMALLRL_PLAYER_H

#include "mob.h"

/* player.c */
void get_speed(int key, int * x, int * y);
int player_move(int input);
int player_turn(void);
void explore(void);

/* Pointer to player controlled mob */
mob_t * player;

#endif
