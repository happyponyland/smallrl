#ifndef SMALLRL_CHAOS_UI_H
#define SMALLRL_CHAOS_UI_H

#include <curses.h>
#include "colors.h"

void chaos_ui_clear(void);
void chaos_ui_messagebox(char *);

void chaos_ui_select_spell_screen(wizard_t *);
int chaos_ui_get_spell_index(wizard_t *);
void chaos_ui_draw_board(game_t *, wizard_t *, point_t *);
cursor_action_t chaos_ui_get_cursor_action(game_t *, point_t);

#endif
