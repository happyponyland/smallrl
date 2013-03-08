#include <stdlib.h>
#include <string.h>

#include "chaos.h"

#include "chaos_internal.h"
#include "chaos_internal_ui.h"
#include "chaos_internal_spells.h"
#include "chaos_internal_startpositions.h"

#define MSGLEN 200

static chaos_outcome_t chaos_play(game_t *);
static void chaos_setup(game_t *, int);
static chaos_outcome_t chaos_game(game_t *);
static void select_spell(wizard_t *);
static void play_round(game_t *, wizard_t *);
static void cast_spell(game_t *, wizard_t *);
static void move_phase(game_t *, wizard_t *);
static void setup_wizard(wizard_t *);
static void place_wizards(game_t *, int);
static void perform_spell(game_t *, wizard_t *, point_t);
static point_t get_position_from_start_position(game_t *, start_position_t);

chaos_outcome_t chaos_duel()
{
    chaos_outcome_t outcome;

    game_t * new_game = malloc(sizeof(game_t));

    outcome = chaos_play(new_game);

    free(new_game);

    return outcome;
}

static chaos_outcome_t chaos_play(game_t * game)
{
    char message[MSGLEN];

    chaos_ui_clear();
    chaos_ui_messagebox("You are magically transported to another dimension.");

    int opponents = 1 + rand() % 4;
    if(opponents > 1)
    {
        snprintf(message, MSGLEN,
                 "You opponent wants more of a challenge and calls in %d extra opponents.",
                 opponents);

        chaos_ui_messagebox(message);
    }

    chaos_setup(game, opponents);

    return chaos_game(game);
}

static void chaos_setup(game_t * game, int number_of_opponents)
{
    game->board_offset.x = 1;
    game->board_offset.y = 1;

    for(int i = 0; i < MAX_WIZARDS_PER_GAME; i += 1)
    {
        game->wizards[i].is_free = true;
    }

    for(int i = 0; i < number_of_opponents + 1; i += 1)
    {
        setup_wizard(&game->wizards[i]);
    }

    game->wizards[0].is_computer = false;
    game->wizards[0].name = "Player";

    place_wizards(game, number_of_opponents + 1);


    for(int i = 0; i < MAX_MONSTERS_PER_GAME; i += 1)
    {
        game->monsters[i].is_free = true;
    }
}

static void place_wizards(game_t * game, int number_of_wizards)
{
    start_position_t * positions;

    switch(number_of_wizards)
    {
        case 2:
            positions = wizard_start_positions_2;
            break;
        case 3:
            positions = wizard_start_positions_3;
            break;
        case 4:
            positions = wizard_start_positions_4;
            break;
        case 5:
            positions = wizard_start_positions_5;
            break;
        case 6:
            positions = wizard_start_positions_6;
            break;
        case 7:
            positions = wizard_start_positions_7;
            break;
        case 8:
            positions = wizard_start_positions_8;
            break;
        case 9:
        default:
            positions = wizard_start_positions_9;
            break;
    }

    for(int i = 0; i < number_of_wizards; i += 1)
    {
        game->wizards[i].position = get_position_from_start_position(game, positions[i]);
    }
}

static point_t get_position_from_start_position(game_t * game, start_position_t start_position)
{
    switch(start_position)
    {
        case start_position_upper_left:
            return (point_t) { .x = 1, .y = 1 };
        case start_position_upper_center:
            return (point_t) { .x = BOARD_WIDTH / 2, .y = 1 };
        case start_position_upper_right:
            return (point_t) { .x = BOARD_WIDTH - 2, .y = 1 };
        case start_position_middle_left:
            return (point_t) { .x = 1, .y = BOARD_HEIGHT / 2 };
        case start_position_middle_center:
            return (point_t) { .x = BOARD_WIDTH / 2, .y = BOARD_HEIGHT / 2 };
        case start_position_middle_right:
            return (point_t) { .x = BOARD_WIDTH - 2, .y = BOARD_HEIGHT / 2 };
        case start_position_lower_left:
            return (point_t) { .x = 1, .y = BOARD_HEIGHT - 2 };
        case start_position_lower_center:
            return (point_t) { .x = BOARD_WIDTH / 2, .y = BOARD_HEIGHT - 2 };
        case start_position_lower_right:
            return (point_t) { .x = BOARD_WIDTH - 2, .y = BOARD_HEIGHT - 2 };

        default:
            return (point_t) { .x = 0, .y = 0 };
    }
}

static void setup_wizard(wizard_t * wizard)
{
    int number_of_spells;

    for(int i = 0; i < MAX_SPELLS_PER_WIZARD; i += 1) {
        wizard->spells[i].is_free = true;
    }

    number_of_spells = 8 + rand() % 5;

    memcpy(&wizard->spells[0], &chaos_spell_disbelieve, sizeof(spell_t));

    for(int i = 1; i < number_of_spells; i += 1) {
        int max = sizeof(chaos_spells) / sizeof(chaos_spells[0]);

        memcpy(&wizard->spells[i], &chaos_spells[rand() % max], sizeof(spell_t));
    }

    wizard->is_alive = true;
    wizard->is_computer = true;
    wizard->name = "Computer";
    wizard->color = color_red;
    wizard->gfx = 'W';

    wizard->is_free = false;
}

static chaos_outcome_t chaos_game(game_t * game)
{
    for(;;)
    {
        int i;
        wizard_t * wizard;

        for(i = 0, wizard = &game->wizards[0]; i < MAX_WIZARDS_PER_GAME; i += 1, wizard += 1) {
            if(wizard->is_alive)
                select_spell(wizard);
        }

        for(i = 0, wizard = &game->wizards[0]; i < MAX_WIZARDS_PER_GAME; i += 1, wizard += 1) {
            if(wizard->is_alive)
                play_round(game, wizard);
        }
    }
}

static void select_spell(wizard_t * wizard)
{
    if(wizard->is_computer)
        return;

    chaos_ui_clear();

    chaos_ui_select_spell_screen(wizard);

    int index = chaos_ui_get_spell_index(wizard);

    wizard->selected_spell = &wizard->spells[index];
}

static void play_round(game_t * game, wizard_t * wizard)
{
    cast_spell(game, wizard);
    move_phase(game, wizard);
}

static void cast_spell(game_t * game, wizard_t * wizard)
{
    point_t cursor_position = wizard->position;
    cursor_action_t cursor_action;

    for(;;) {
        chaos_ui_draw_board(game, wizard, &cursor_position);

        cursor_action = chaos_ui_get_cursor_action(game, cursor_position);

        if(cursor_action == cursor_action_fire) {
            perform_spell(game, wizard, cursor_position);
            break;
        }

        switch(cursor_action)
        {
            case cursor_action_move_up:
                if (cursor_position.y > 0)
                    cursor_position.y -= 1;
                break;
            case cursor_action_move_down:
                if(cursor_position.y < BOARD_HEIGHT - 1)
                    cursor_position.y += 1;
                break;
            case cursor_action_move_left:
                if(cursor_position.x > 0)
                    cursor_position.x -= 1;
                break;
            case cursor_action_move_right:
                if(cursor_position.x < BOARD_WIDTH - 1)
                    cursor_position.x += 1;
                break;
            default:
                break;
        }
    }
}

static void perform_spell(game_t * game, wizard_t * wizard, point_t target)
{
}


static void move_phase(game_t * game, wizard_t * wizard)
{
}
