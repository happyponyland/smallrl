#ifndef SMALLRL_GAME_H
#define SMALLRL_GAME_H

#include "mob.h"

#define TURN_ABORT (-1)
#define TURN_COMPLETE 0
#define TURN_QUIT 1
#define TURN_DESCEND 2

/* game.c */
void new_game(void);
int play(void);

#endif
