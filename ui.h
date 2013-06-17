#ifndef SMALLRL_UI_H
#define SMALLRL_UI_H

#include "level.h"
#include "colors.h"

#include "input_type.h"
#include "player.h"
#include "log.h"

/* Size of default message buffer */
#define MSGLEN 200

/* ui.c */
void draw_map(input_type_t, level_t *);
void draw_stats(player_info_t *, level_t *);
int prompt_yn(char *);
void print_msg(char *);
void clear_msg(void);
void wait(void);

void draw_log(input_type_t, log_t *, level_t *);

#endif
