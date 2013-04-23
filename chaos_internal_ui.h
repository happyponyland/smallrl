#ifndef SMALLRL_CHAOS_UI_H
#define SMALLRL_CHAOS_UI_H

#include <curses.h>
#include "colors.h"

typedef struct gfx_s
{
    color_t color;
    char gfx;
} gfx_t;

void chaos_ui_clear(void);
void chaos_ui_messagebox(char *);

void chaos_ui_select_spell_screen(wizard_t *);
int chaos_ui_get_spell_index(wizard_t *);
void chaos_ui_draw_board(game_t *, wizard_t *);
void chaos_ui_draw_status_selected_spell(game_t *, wizard_t *);
cursor_action_t chaos_ui_get_cursor_action(game_t *, point_t);
void chaos_ui_draw_status_active_wizard(game_t *, wizard_t *);
void chaos_ui_draw_status_active_monster(game_t *, monster_t *);
void chaos_ui_draw_status_active_wizard_ranged(game_t *, wizard_t *);
void chaos_ui_draw_status_active_monster_ranged(game_t *, monster_t *);
void chaos_ui_clear_status(game_t *);
void chaos_ui_draw_status(game_t *, color_t, char *);
void chaos_ui_draw_status_with_delay(game_t *, color_t, char *, int);

#endif
