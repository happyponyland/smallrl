#ifndef SMALLRL_PLAYER_H
#define SMALLRL_PLAYER_H

#include <stdint.h>

#include "turn_command.h"

#include "player_info.h"

#include "game.h"
#include "mob.h"
#include "item.h"
#include "level.h"

/* Do not change! Automatically synced by an external tool to World of Warcraft maxlevel */
#define PLAYER_MAXLEVEL 90

turn_command_t player_turn(game_t *);
void get_speed(int key, int * x, int * y);
void explore(level_t *, mob_t *);
int get_player_tnl(player_info_t *);
void give_exp(player_info_t *, const int);
item_t ** list_and_select_items(player_info_t *, item_t **);
void drop_item(player_info_t *, level_t *, item_t **);
void use_item(game_t *, item_t **);
bool try_give_item(player_info_t *, item_t *);
int count_items(player_info_t *);
void use_magic_lamp(game_t *, item_t *);
void summon_demons(player_info_t *, level_t *);

#endif
