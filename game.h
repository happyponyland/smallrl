#ifndef SMALLRL_GAME_H
#define SMALLRL_GAME_H

#include "log.h"
#include "player_info.h"
#include "input_type.h"
#include "turn_command.h"
#include "level.h"

typedef struct game_s game_t;
struct game_s
{
    level_t * level;
    player_info_t player;
    log_t log;

    input_type_t input_type;
};

game_t * new_game(void);
void free_game(game_t *);
int play(game_t *);

bool try_move_mob(game_t *, level_t *, mob_t *, int, int);

turn_command_t execute_command(game_t *, char *);

#endif
