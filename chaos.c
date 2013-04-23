#include <stdlib.h>
#include <string.h>

#include "chaos.h"

#include "chaos_internal.h"
#include "chaos_internal_ui.h"
#include "chaos_internal_ai.h"
#include "chaos_internal_spells.h"
#include "chaos_internal_startpositions.h"
#include "chaos_internal_conjure.h"

static chaos_outcome_t chaos_play(game_t *);
static void chaos_setup(game_t *, int);
static chaos_outcome_t chaos_game(game_t *);
static void select_spell(wizard_t *);
static void play_round(game_t *, wizard_t *);
static void cast_spell(game_t *, wizard_t *);
static void move_phase(game_t *, wizard_t *);
static void setup_wizard(wizard_t *);
static void place_wizards(game_t *, int);
static bool perform_spell(game_t *, wizard_t *, point_t);
static point_t get_position_from_start_position(start_position_t);
static wizard_t * get_wizard_at_position(game_t *, point_t);
static monster_t * get_monster_at_position(game_t *, point_t);
static void setup_wizard_round(wizard_t *);
static void setup_monster_round(monster_t *);
static void shuffle_colors(color_t *, size_t);
static attack_outcome_t attack_monster(game_t *, monster_t *, monster_t *);
static attack_outcome_t attack_wizard(game_t *, monster_t *, wizard_t *);
static int max(int, int);
static void monster_kill_monster(game_t *, monster_t *, monster_t *);
static void monster_kill_wizard(game_t *, monster_t *, wizard_t *);
static void kill_monster(monster_t *);
static void kill_wizard(wizard_t *);
static attack_outcome_t monster_shoot_monster(game_t *, monster_t *, monster_t *);
static attack_outcome_t monster_shoot_wizard(game_t *, monster_t *, wizard_t *);
static attack_outcome_t perform_attack(int, int);
static int count_wizards_alive(game_t *);

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
    char message[CHAOS_MSGLEN];

    chaos_ui_clear();
    chaos_ui_messagebox("You are magically transported to another dimension.");

    int opponents = 1 + rand() % 4;
    if(opponents > 1)
    {
        snprintf(message, CHAOS_MSGLEN,
                 "Your opponent wants more of a challenge and calls in %d extra opponents.",
                 opponents);

        chaos_ui_messagebox(message);
    }

    chaos_setup(game, opponents);

    return chaos_game(game);
}

static void chaos_setup(game_t * game, int number_of_opponents)
{
    static color_t available_colors[] = { color_yellow,
                                   color_blue,
                                   color_red,
                                   color_green,
                                   color_cyan,
                                   color_magenta
    };

    size_t number_of_colors = sizeof(available_colors) / sizeof(color_t);

    shuffle_colors(available_colors, number_of_colors);

    game->board_offset.x = 1;
    game->board_offset.y = 1;

    for(int i = 0; i < MAX_WIZARDS_PER_GAME; i += 1)
    {
        game->wizards[i].is_free = true;
        game->wizards[i].moves_per_round = 1;
        game->wizards[i].color = available_colors[i % number_of_colors];
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

    return;
}

static void shuffle_colors(color_t * available_colors, size_t size)
{
    if(size <= 1)
        return;

    size_t i;
    for (i = 0; i < size - 1; i += 1)
    {
        size_t new_index = i + rand() / (RAND_MAX / (size - i) + 1);
        color_t temp = available_colors[new_index];
        available_colors[new_index] = available_colors[i];
        available_colors[i] = temp;
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
        game->wizards[i].position = get_position_from_start_position(positions[i]);
    }

    return;
}

static point_t get_position_from_start_position(start_position_t start_position)
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
    wizard->gfx = 'W';

    wizard->stat_range = 0;
    wizard->stat_ranged_combat = 0;
    wizard->stat_combat = 1;
    wizard->stat_defence = 1;
    wizard->stat_magic_resistance = 1;
    wizard->stat_manoeuvre_rating = 1;

    wizard->is_free = false;

    return;
}

static int count_wizards_alive(game_t * game)
{
    int i;
    int count;
    wizard_t * wizard;

    count = 0;
    for(i = 0, wizard = &game->wizards[0]; i < MAX_WIZARDS_PER_GAME; i += 1, wizard += 1) {
        if(!wizard->is_free && wizard->is_alive)
        {
            count += 1;
        }
    }

    return count;
}

static chaos_outcome_t chaos_game(game_t * game)
{
    for(;;)
    {
        int i;
        wizard_t * wizard;
        monster_t * monster;
        int count;

        count = count_wizards_alive(game);
        if(count == 0)
        {
            return chaos_outcome_draw;
        } else if( count == 1) {
            if(game->wizards[0].is_alive) {
                return chaos_outcome_win;
            } else {
                return chaos_outcome_loss;
            }
        }

        for(i = 0, wizard = &game->wizards[0]; i < MAX_WIZARDS_PER_GAME; i += 1, wizard += 1) {
            if(!wizard->is_free && wizard->is_alive)
                setup_wizard_round(wizard);
        }

        for(i = 0, monster = &game->monsters[0]; i < MAX_MONSTERS_PER_GAME; i += 1, monster += 1) {
            if(!(monster->is_free || monster->is_dead))
                setup_monster_round(monster);
        }

        for(i = 0, wizard = &game->wizards[0]; i < MAX_WIZARDS_PER_GAME; i += 1, wizard += 1) {
            if(!wizard->is_free && wizard->is_alive)
                select_spell(wizard);
        }

        for(i = 0, wizard = &game->wizards[0]; i < MAX_WIZARDS_PER_GAME; i += 1, wizard += 1) {
            if(!wizard->is_free && wizard->is_alive)
                play_round(game, wizard);
        }
    }

    return chaos_outcome_draw;
}

static void setup_wizard_round(wizard_t * wizard)
{
    wizard->moves_left = wizard->moves_per_round;
}

static void setup_monster_round(monster_t * monster)
{
    monster->moves_left = monster->moves_per_round;
}

static void select_spell(wizard_t * wizard)
{
    if(wizard->is_computer)
    {
        chaos_ai_select_spell(wizard);
        return;
    }

    chaos_ui_clear();

    chaos_ui_select_spell_screen(wizard);

    int index = chaos_ui_get_spell_index(wizard);

    wizard->selected_spell = &wizard->spells[index];

    return;
}

static void play_round(game_t * game, wizard_t * wizard)
{
    cast_spell(game, wizard);
    move_phase(game, wizard);

    return;
}

static void cast_spell(game_t * game, wizard_t * wizard)
{
    point_t cursor_position = wizard->position;
    cursor_action_t cursor_action;

    if(wizard->is_computer)
    {
        return;
    }

    for(;;) {
        chaos_ui_draw_board(game, wizard);
        chaos_ui_clear_status(game);
        chaos_ui_draw_status_selected_spell(game, wizard);

        cursor_action = chaos_ui_get_cursor_action(game, cursor_position);

        if(cursor_action == cursor_action_fire) {
            if(perform_spell(game, wizard, cursor_position))
            {
                break;
            }
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

    return;
}

static bool perform_spell(game_t * game, wizard_t * wizard, point_t target)
{
    switch(wizard->selected_spell->type)
    {
        case spell_type_conjure:
            if(target.x == wizard->position.x && target.y == wizard->position.y)
            {
                return false;
            }

            if(abs(wizard->position.x - target.x) > 1 || abs(wizard->position.y - target.y) > 1)
            {
                return false;
            }

            if(get_monster_at_position(game, target) != NULL)
            {
                return false;
            }

            if(get_wizard_at_position(game, target) != NULL)
            {
                return false;
            }

            chaos_conjure(wizard->selected_spell->subtype, game, wizard, target);
            break;
        default:
            break;
    }

    wizard->selected_spell->is_free = true;

    return true;
}

//...kiiiilllllll meeee---.....
static void move_phase(game_t * game, wizard_t * wizard)
{
    point_t cursor_position = wizard->position;
    cursor_action_t cursor_action;
    wizard_t * active_wizard = NULL;
    monster_t * active_monster = NULL;
    bool is_ranged_phase;

    if(wizard->is_computer) {
        return;
    }

    for(;;) {
        chaos_ui_draw_board(game, wizard);
        chaos_ui_clear_status(game);
        if(active_wizard != NULL) {
            if(is_ranged_phase) {
                chaos_ui_draw_status_active_wizard_ranged(game, active_wizard);
            } else {
                chaos_ui_draw_status_active_wizard(game, active_wizard);
            }
        }
        else if(active_monster != NULL)
            if(is_ranged_phase) {
                chaos_ui_draw_status_active_monster_ranged(game, active_monster);
            } else {
                chaos_ui_draw_status_active_monster(game, active_monster);
            }
        else
        {
            chaos_ui_draw_status(game, wizard->color, "Move phase");
        }

        cursor_action = chaos_ui_get_cursor_action(game, cursor_position);

        if(cursor_action == cursor_action_done) {
           break;
        }

        if(cursor_action == cursor_action_fire) {
            if(active_monster == NULL && active_wizard == NULL) {
                active_wizard = get_wizard_at_position(game, cursor_position);
                active_monster = get_monster_at_position(game, cursor_position);

                if(active_wizard == NULL && active_monster == NULL)
                    continue;

                if(active_monster == NULL)
                {
                    if(active_wizard != wizard)
                    {
                        active_wizard = NULL;
                        continue;
                    }

                    if(active_wizard->moves_left == 0)
                    {
                        active_wizard = NULL;
                        continue;
                    }
                }

                if(active_wizard == NULL)
                {
                    if(active_monster->is_dead)
                    {
                        active_monster = NULL;
                        continue;
                    }

                    if(active_monster->owner != wizard)
                    {
                        active_monster = NULL;
                        continue;
                    }

                    if(active_monster->moves_left == 0)
                    {
                        active_monster = NULL;
                        continue;
                    }
                }

                is_ranged_phase = false;
            } else if(active_monster == NULL && active_wizard != NULL) {
                if(!is_ranged_phase) {
                    if(active_wizard->moves_left == 0)
                    {
                        active_wizard = NULL;
                        continue;
                    }

                    if(cursor_position.x == active_wizard->position.x && cursor_position.y == active_wizard->position.y)
                    {
                        continue;
                    }

                    if(abs(active_wizard->position.x - cursor_position.x) > 1 || abs(active_wizard->position.y - cursor_position.y) > 1)
                    {
                        continue;
                    }

                    active_wizard->position = cursor_position;
                    active_wizard->moves_left -= 1;

                    if(active_wizard->moves_left == 0)
                    {
                        active_wizard = NULL;
                        continue;
                    }
                }
            }
            else if(active_monster != NULL && active_wizard == NULL)
            {
                if(is_ranged_phase)
                {
                    int length = max(abs(active_monster->position.x - cursor_position.x), abs(active_monster->position.y - cursor_position.y));

                    if(length > active_monster->stat_range)
                    {
                        continue;
                    }

                    // Attack?
                    monster_t * monster_defender = get_monster_at_position(game, cursor_position);
                    wizard_t * wizard_defender = get_wizard_at_position(game, cursor_position);

                    if(monster_defender != NULL) {
                        if(monster_shoot_monster(game, active_monster, monster_defender) == attack_outcome_win)
                        {
                            kill_monster(monster_defender);
                        }
                    } else if(wizard_defender != NULL) {
                        if(monster_shoot_wizard(game, active_monster, wizard_defender) == attack_outcome_win)
                        {
                            kill_wizard(wizard_defender);
                        }
                    }

                    is_ranged_phase = false;
                    active_monster = NULL;
                    continue;
                }

                if(!is_ranged_phase)
                {
                    if(active_monster->moves_left == 0)
                    {
                        active_monster = NULL;
                        continue;
                    }

                    if(cursor_position.x == active_monster->position.x && cursor_position.y == active_monster->position.y)
                    {
                        continue;
                    }

                    if(active_monster->move_type == move_type_walking)
                    {
                        if(abs(active_monster->position.x - cursor_position.x) > 1 || abs(active_monster->position.y - cursor_position.y) > 1)
                        {
                            continue;
                        }

                        // Attack?
                        monster_t * monster_defender = get_monster_at_position(game, cursor_position);
                        wizard_t * wizard_defender = get_wizard_at_position(game, cursor_position);

                        if(monster_defender != NULL)
                        {
                            if(monster_defender->owner == active_wizard)
                            {
                                continue;
                            }

                            if(attack_monster(game, active_monster, monster_defender) == attack_outcome_loss)
                            {
                                active_monster->moves_left -= 1;

                                if(active_monster->moves_left == 0)
                                {
                                    active_monster = NULL;
                                }

                                continue;
                            }
                        }

                        if(wizard_defender != NULL)
                        {
                            if(wizard_defender == active_wizard)
                            {
                                continue;
                            }

                            if(attack_wizard(game, active_monster, wizard_defender) == attack_outcome_loss)
                            {
                                active_monster->moves_left -= 1;

                                if(active_monster->moves_left == 0)
                                {
                                    if(active_monster->ranged_type != ranged_type_none) {
                                        is_ranged_phase = true;
                                        continue;
                                    }

                                    active_monster = NULL;
                                }

                                continue;
                            }
                            else
                            {
                                active_monster->moves_left = 0;
                                active_monster->position = cursor_position;
                                active_monster = NULL;
                                continue;
                            }
                        }

                        active_monster->position = cursor_position;
                        active_monster->moves_left -= 1;
                    }
                    else if(active_monster->move_type == move_type_flying)
                    {
                        int length = max(abs(active_monster->position.x - cursor_position.x), abs(active_monster->position.y - cursor_position.y));

                        if(length > active_monster->moves_left)
                        {
                            continue;
                        }

                        // Attack?
                        monster_t * monster_defender = get_monster_at_position(game, cursor_position);
                        wizard_t * wizard_defender = get_wizard_at_position(game, cursor_position);

                        if(monster_defender != NULL)
                        {
                            if(monster_defender->owner == active_wizard)
                            {
                                continue;
                            }

                            if(attack_monster(game, active_monster, monster_defender) == attack_outcome_loss)
                            {
                                active_monster->moves_left = 0;
                                active_monster = NULL;

                                continue;
                            }
                        }

                        if(wizard_defender != NULL)
                        {
                            if(wizard_defender == active_wizard)
                            {
                                continue;
                            }

                            if(attack_wizard(game, active_monster, wizard_defender) == attack_outcome_loss)
                            {
                                active_monster->moves_left = 0;
                                active_monster = NULL;

                                continue;
                            }
                        }

                        active_monster->position = cursor_position;
                        active_monster->moves_left = 0;

                        if(active_monster->ranged_type != ranged_type_none) {
                            is_ranged_phase = true;
                            continue;
                        }

                    }
                }

                if(active_monster->moves_left == 0)
                {
                    if(active_monster->ranged_type != ranged_type_none) {
                        is_ranged_phase = true;
                        continue;
                    }

                    active_monster = NULL;
                    continue;
                }
            }
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

    return;
}

static inline int max(int first, int second)
{
    return first > second ? first : second;
}

static attack_outcome_t attack_monster(game_t * game, monster_t * attacker, monster_t * defender)
{
    attack_outcome_t outcome = perform_attack(attacker->stat_combat, defender->stat_defence);

    if(outcome == attack_outcome_win) {
        monster_kill_monster(game, attacker, defender);
    }

    return outcome;
}

static attack_outcome_t attack_wizard(game_t * game, monster_t * attacker, wizard_t * defender)
{
    attack_outcome_t outcome = perform_attack(attacker->stat_combat, defender->stat_defence);

    if(outcome == attack_outcome_win) {
        monster_kill_wizard(game, attacker, defender);
    }

    return outcome;
}

static void monster_kill_monster(game_t * game, monster_t * attacker, monster_t * monster)
{
    char message[CHAOS_MSGLEN];

    snprintf(message, CHAOS_MSGLEN,
             "%s kills %s.",
             attacker->name, monster->name);

    chaos_ui_clear_status(game);
    chaos_ui_draw_status_with_delay(game, attacker->owner->color, message, 500);

    kill_monster(monster);
}

static void kill_monster(monster_t * monster)
{
    if((monster->monster_type & monster_type_undead) == monster_type_undead) {
        monster->is_free = true;
        return;
    }

    monster->is_dead = true;
}

static void kill_wizard(wizard_t * wizard)
{
    wizard->is_free = true;
    wizard->is_alive = false;
}


static void monster_kill_wizard(game_t * game, monster_t * attacker, wizard_t * wizard)
{
    char message[CHAOS_MSGLEN];

    snprintf(message, CHAOS_MSGLEN,
             "%s kills %s.",
             attacker->name, wizard->name);

    chaos_ui_clear_status(game);
    chaos_ui_draw_status_with_delay(game, attacker->owner->color, message, 500);

    kill_wizard(wizard);
}

static wizard_t * get_wizard_at_position(game_t * game, point_t point)
{
    int i;
    wizard_t * wizard;

    for(i = 0, wizard = &game->wizards[0]; i < MAX_WIZARDS_PER_GAME; i += 1, wizard += 1) {
        if(wizard->is_free || !wizard->is_alive)
        {
            continue;
        }

        if(wizard->position.x == point.x && wizard->position.y == point.y)
        {
            return wizard;
        }
    }

    return NULL;
}

static monster_t * get_monster_at_position(game_t * game, point_t point)
{
    int i;
    monster_t * monster;

    for(i = 0, monster = &game->monsters[0]; i < MAX_MONSTERS_PER_GAME; i += 1, monster += 1) {
        if(monster->is_free)
        {
            continue;
        }

        if(monster->position.x == point.x && monster->position.y == point.y)
        {
            return monster;
        }
    }

    return NULL;
}

static attack_outcome_t perform_attack(int attack, int defence)
{
    int random_attack = rand() % 11;
    int random_defence = rand() % 11;

    attack += random_attack;
    defence += random_defence;

    if(attack < defence)
    {
        return attack_outcome_loss;
    }

    return attack_outcome_win;
}

static attack_outcome_t monster_shoot_monster(game_t * game, monster_t * attacker, monster_t * defender)
{
    //TODO: Draw animation

    attack_outcome_t outcome = perform_attack(attacker->stat_ranged_combat, defender->stat_defence);

    if(outcome == attack_outcome_win) {
        monster_kill_monster(game, attacker, defender);
    }

    return outcome;
}

static attack_outcome_t monster_shoot_wizard(game_t * game, monster_t * attacker, wizard_t * defender)
{
    attack_outcome_t outcome = perform_attack(attacker->stat_ranged_combat, defender->stat_defence);

    if(outcome == attack_outcome_win) {
        monster_kill_wizard(game, attacker, defender);
    }

    return outcome;
}
