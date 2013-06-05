#ifndef SMALLRL_UI_H
#define SMALLRL_UI_H

#include "level.h"
#include "colors.h"

/* Size of default message buffer */
#define MSGLEN 200

/* ui.c */
void draw_map(level_t *);
void draw_stats(level_t *);
int prompt_yn(char *);
void print_msg(char *);
void clear_msg(void);
void wait(void);

void reset_log(void);
void draw_log(level_t *);

#endif
