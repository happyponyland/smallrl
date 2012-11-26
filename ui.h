#ifndef SMALLRL_UI_H
#define SMALLRL_UI_H

#include "level.h"

typedef enum color_e
{
    color_black,
    color_yellow,
    color_blue,
    color_red,
    color_green,
    color_cyan,
    color_magenta
} color_t;

/* ui.c */
void draw_map(level_t *);
void draw_stats(mob_t *, level_t *);
int prompt_yn(char *);
void print_msg(char *);
void clear_msg(void);
void wait(void);

#endif
