#ifndef SMALLRL_UI_H
#define SMALLRL_UI_H

#include "level.h"
#include "colors.h"

/* Size of default message buffer */
#define MSGLEN 200

typedef enum ui_input_type_e
{
    ui_input_type_map,
    ui_input_type_log,
} ui_input_type_t;

extern ui_input_type_t current_ui_input_type;

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
