#include <string.h>

#include "chaos_internal.h"
#include "chaos_internal_ui.h"

static char get_spell_order_char(spell_t *);
static gfx_t get_gfx_at_position(game_t *, point_t);
static bool same_position(point_t, point_t);

void chaos_ui_clear()
{
    clear();

    return;
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

    for(int i = 1; i < height; i += 1) {
        move(y + i, x + 1);
        hline(' ', width - 1);
    }

    move(y, x);
    addch(ACS_ULCORNER);

    move(y, x + width);
    addch(ACS_URCORNER);

    move(y + height, x);
    addch(ACS_LLCORNER);

    move(y + height, x + width);
    addch(ACS_LRCORNER);

    move(y + padding_y / 2 + border_height, x + padding_x / 2 + border_width);
    addstr(message);

    getch();

    return;
}

void chaos_ui_select_spell_screen(wizard_t * wizard)
{
    char message[CHAOS_MSGLEN];

    int col, row;

    row = 1;
    col = 5;

    int spell_count = 0;

    for(int i = 0; i < MAX_SPELLS_PER_WIZARD; i += 1) {
        if(wizard->spells[i].is_free)
            continue;

        move(row++, col);

        snprintf(message, CHAOS_MSGLEN, "%c. %c %s", '1' + spell_count, get_spell_order_char(&wizard->spells[i]), wizard->spells[i].name);
        addstr(message);

        spell_count += 1;
    }

    return;
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

            spell_count += 1;
        }
    }

    return -1;
}

void chaos_ui_draw_board(game_t * game, wizard_t * current_wizard)
{
    int x,y;

    int offset_x = game->board_offset.x;
    int offset_y = game->board_offset.y;

    if(current_wizard != NULL)
        attron(COLOR_PAIR(current_wizard->color));

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
            move(1 + offset_y + y, 1 + offset_x + x);
            gfx_t gfx = get_gfx_at_position(game, (point_t) { .x = x, .y = y });
            attron(COLOR_PAIR(gfx.color));
            addch(gfx.gfx);
        }
    }

    return;
}

static bool same_position(point_t first, point_t second)
{
    return first.x == second.x && first.y == second.y;
}

static gfx_t get_gfx_at_position(game_t * game, point_t position)
{
    int i;

    for(i = 0; i < MAX_WIZARDS_PER_GAME; i += 1)
    {
        if(game->wizards[i].is_free)
            continue;

        if(same_position(game->wizards[i].position, position))
            return (gfx_t) { .gfx = game->wizards[i].gfx, .color = game->wizards[i].color };
    }

    for(i = 0; i < MAX_MONSTERS_PER_GAME; i += 1)
    {
        if(game->monsters[i].is_free)
            continue;

        if(same_position(game->monsters[i].position, position))
            return (gfx_t) { .gfx = game->monsters[i].gfx, .color = game->monsters[i].color };
    }

    return (gfx_t) { .gfx = ' ', .color = color_black };
}

void chaos_ui_draw_status_selected_spell(game_t * game, wizard_t * wizard)
{
    char message[CHAOS_MSGLEN];

    move(game->board_offset.y + BOARD_HEIGHT + 2, 0);

    snprintf(message, CHAOS_MSGLEN, "%s casting %s.", wizard->name, wizard->selected_spell->name);

    attron(COLOR_PAIR(wizard->color));
    addstr(message);

    return;
}

void chaos_ui_draw_status_active_wizard(game_t * game, wizard_t * wizard)
{
    char message[CHAOS_MSGLEN];

    move(game->board_offset.y + BOARD_HEIGHT + 2, 0);

    snprintf(message, CHAOS_MSGLEN, "%s moving.", wizard->name);

    attron(COLOR_PAIR(wizard->color));
    addstr(message);

    return;
}

void chaos_ui_draw_status_active_monster(game_t * game, monster_t * monster)
{
    char message[CHAOS_MSGLEN];

    move(game->board_offset.y + BOARD_HEIGHT + 2, 0);

    snprintf(message, CHAOS_MSGLEN, "%s (%s) moving.", monster->name, monster->owner->name);

    attron(COLOR_PAIR(monster->owner->color));
    addstr(message);

    return;
}

void chaos_ui_draw_status_active_wizard_ranged(game_t * game, wizard_t * wizard)
{
    char message[CHAOS_MSGLEN];

    move(game->board_offset.y + BOARD_HEIGHT + 2, 0);

    snprintf(message, CHAOS_MSGLEN, "%s shoot!", wizard->name);

    attron(COLOR_PAIR(wizard->color));
    addstr(message);

    return;
}

void chaos_ui_draw_status_active_monster_ranged(game_t * game, monster_t * monster)
{
    char message[CHAOS_MSGLEN];

    move(game->board_offset.y + BOARD_HEIGHT + 2, 0);

    snprintf(message, CHAOS_MSGLEN, "%s (%s) shoot!", monster->name, monster->owner->name);

    attron(COLOR_PAIR(monster->owner->color));
    addstr(message);

    return;
}

void chaos_ui_draw_status(game_t * game, color_t color, char * message)
{
    move(game->board_offset.y + BOARD_HEIGHT + 2, 0);

    attron(COLOR_PAIR(color));
    addstr(message);

    return;
}

void chaos_ui_draw_status_with_delay(game_t * game, color_t color, char * message, int milliseconds_delay)
{
    move(game->board_offset.y + BOARD_HEIGHT + 2, 0);

    attron(COLOR_PAIR(color));
    addstr(message);

    if(milliseconds_delay < 0) {
        milliseconds_delay = 0;
    }

    timeout(milliseconds_delay);

    getch();

    timeout(-1);

    return;
}

void chaos_ui_clear_status(game_t * game)
{
    move(game->board_offset.y + BOARD_HEIGHT + 2, 0);
    clrtoeol();

    return;
}

cursor_action_t chaos_ui_get_cursor_action(game_t * game, point_t position)
{
    int input;
    cursor_action_t result;
    gfx_t gfx_under_cursor = get_gfx_at_position(game, position);
    bool draw_cursor = false;

    timeout(500);

    for(;;)
    {
        move(1 + game->board_offset.y + position.y, 1 + game->board_offset.x + position.x);
        if(draw_cursor ^= true)
        {
            attron(COLOR_PAIR(color_white));
            addch(ACS_CKBOARD);
        }
        else
        {
            attron(COLOR_PAIR(gfx_under_cursor.color));
            addch(gfx_under_cursor.gfx);
        }

        input = getch();
        if(input != ERR)
        {
            if(input == KEY_ENTER || input == ' ') {
                result = cursor_action_fire;
                break;
            }

            if(input == 'd' || input == 'D') {
                result = cursor_action_done;
                break;
            }

            if(input == KEY_UP) {
                result = cursor_action_move_up;
                break;
            }

            if(input == KEY_RIGHT) {
                result = cursor_action_move_right;
                break;
            }

            if(input == KEY_DOWN) {
                result = cursor_action_move_down;
                break;
            }

            if(input == KEY_LEFT) {
                result = cursor_action_move_left;
                break;
            }
        }
    }

    timeout(-1);

    return result;
}
