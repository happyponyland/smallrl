#include <string.h>

#include "chaos_internal.h"
#include "chaos_internal_ui.h"

static char get_spell_order_char(spell_t *);
static char get_gfx_at_position(game_t *, point_t);
static bool same_position(point_t, point_t);

void chaos_ui_clear()
{
    clear();
}

void chaos_ui_messagebox(char * message)
{
    int x, y, width, height;
    int message_length, message_lines;
    int padding_x, padding_y;
    int border_width, border_height;


    padding_x = padding_y = 2;
    border_width = border_height = 1;

    message_length = strlen(message);
    message_lines = 1;

    width = message_length + padding_x + border_width;
    height = message_lines + padding_y + border_height;

    x = COLS / 2 - (message_length / 2 + padding_x + border_width);
    y = LINES / 2 - (message_lines / 2 + padding_y + border_height);

    move(y, x);
    hline(ACS_HLINE, width);
    move(y, x + width);
    vline(ACS_VLINE, height);
    move(y,x);
    vline(ACS_VLINE, height);
    move(y + height, x);
    hline(ACS_HLINE, width);

    move(y + padding_y / 2 + border_height, x + padding_x / 2 + border_width);
    addstr(message);

    getch();
}

void chaos_ui_select_spell_screen(wizard_t * wizard)
{
    char message[50];

    int col, row;

    row = 1;
    col = 5;

    int spell_count = 0;

    for(int i = 0; i < MAX_SPELLS_PER_WIZARD; i += 1) {
        if(wizard->spells[i].is_free)
            continue;

        move(row++, col);

        sprintf(message, "%c. %c %s", '1' + spell_count, get_spell_order_char(&wizard->spells[i]), wizard->spells[i].name);
        addstr(message);

        spell_count += 1;
    }
}

static char get_spell_order_char(spell_t * spell)
{
    switch(spell->order)
    {
        case spell_order_law:
            return '^';
        case spell_order_chaos:
            return '*';
        case spell_order_neutral:
            return '-';
    }

    return '?';
}

int chaos_ui_get_spell_index(wizard_t * wizard)
{
    int input;

    for(;;) {
        input = getch() - '1';

        int spell_count = 0;

        for(int i = 0; i < MAX_SPELLS_PER_WIZARD; i += 1) {
            if(wizard->spells[i].is_free)
                continue;

            if(input == spell_count)
                return i;

            spell_count -= 1;
        }
    }
}

void chaos_ui_draw_board(game_t * game, wizard_t * current_wizard, point_t * cursor_position)
{
    int x,y;

    int offset_x = game->board_offset.x;
    int offset_y = game->board_offset.y;

    for(y = 0; y < BOARD_HEIGHT + 2; y += 1)
    {
        move(offset_y + y, offset_x);
        addch(ACS_CKBOARD);
        move(offset_y + y, offset_x + BOARD_WIDTH + 1);
        addch(ACS_CKBOARD);
    }

    for(x = 0; x < BOARD_WIDTH + 2; x += 1)
    {
        move(offset_y, offset_x + x);
        addch(ACS_CKBOARD);
        move(offset_y + BOARD_HEIGHT + 1, offset_x + x);
        addch(ACS_CKBOARD);
    }

    for(y = 0; y < BOARD_HEIGHT; y += 1)
    {
        for(x = 0; x < BOARD_WIDTH; x += 1)
        {
            move(1 + offset_y + y, 1 + offset_x +x);
            addch(get_gfx_at_position(game, (point_t) { .x = x, .y = y }));
        }
    }
}

static bool same_position(point_t first, point_t second)
{
    return first.x == second.x && first.y == second.y;
}

static char get_gfx_at_position(game_t * game, point_t position)
{
    int i;

    for(i = 0; i < MAX_WIZARDS_PER_GAME; i += 1)
    {
        if(game->wizards[i].is_free)
            continue;

        if(same_position(game->wizards[i].position, position))
            return game->wizards[i].gfx;
    }

    for(i = 0; i < MAX_MONSTERS_PER_GAME; i += 1)
    {
        if(game->monsters[i].is_free)
            continue;

        if(same_position(game->monsters[i].position, position))
            return game->monsters[i].gfx;
    }

    return ' ';
}

cursor_action_t chaos_ui_get_cursor_action(game_t * game, point_t position)
{
    int input;
    char gfx_under_cursor = get_gfx_at_position(game, position);
    bool draw_cursor = false;

    timeout(500);

    for(;;)
    {
        move(1 + game->board_offset.y + position.y, 1 + game->board_offset.x + position.x);
        if(draw_cursor ^= true)
            addch(ACS_CKBOARD);
        else
            addch(gfx_under_cursor);

        input = getch();
        if(input != ERR)
        {
            if(input == KEY_ENTER || input == ' ')
                return cursor_action_fire;

            if(input == KEY_UP)
                return cursor_action_move_up;

            if(input == KEY_RIGHT)
                return cursor_action_move_right;

            if(input == KEY_DOWN)
                return cursor_action_move_down;

            if(input == KEY_LEFT)
                return cursor_action_move_left;
        }
    }

    timeout(-1);
}
