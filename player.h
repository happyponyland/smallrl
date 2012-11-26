#ifndef SMALLRL_PLAYER_H
#define SMALLRL_PLAYER_H

#include "mob.h"

/* player.c */
int player_turn(void);
void explore(void);

/* Pointer to player controlled mob */
mob_t * player;

#endif
