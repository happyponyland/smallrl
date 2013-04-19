#include <stdlib.h>

#include "chaos_internal_conjure.h"

static monster_t * conjure_bat(game_t *, wizard_t *, point_t);
static monster_t * conjure_bear(game_t *, wizard_t *, point_t);
static monster_t * conjure_ghost(game_t *, wizard_t *, point_t);
static monster_t * conjure_zombie(game_t *, wizard_t *, point_t);
static monster_t * conjure_golden_dragon(game_t *, wizard_t *, point_t);
static monster_t * conjure_centaur(game_t *, wizard_t *, point_t);
static monster_t * conjure_green_dragon(game_t *, wizard_t *, point_t);
static monster_t * conjure_hydra(game_t *, wizard_t *, point_t);
static monster_t * conjure_skeleton(game_t *, wizard_t *, point_t);
static monster_t * conjure_ogre(game_t *, wizard_t *, point_t);
static monster_t * conjure_goblin(game_t *, wizard_t *, point_t);
static monster_t * conjure_king_cobra(game_t *, wizard_t *, point_t);
static monster_t * conjure_wraith(game_t *, wizard_t *, point_t);
static monster_t * conjure_harpy(game_t *, wizard_t *, point_t);
static monster_t * conjure_giant_rat(game_t *, wizard_t *, point_t);
static monster_t * conjure_unicorn(game_t *, wizard_t *, point_t);
static monster_t * conjure_orc(game_t *, wizard_t *, point_t);
static monster_t * conjure_giant(game_t *, wizard_t *, point_t);
static monster_t * conjure_gryphon(game_t *, wizard_t *, point_t);
static monster_t * conjure_pegasus(game_t *, wizard_t *, point_t);
static monster_t * conjure_gorilla(game_t *, wizard_t *, point_t);
static monster_t * conjure_manticore(game_t *, wizard_t *, point_t);
static monster_t * conjure_red_dragon(game_t *, wizard_t *, point_t);
static monster_t * conjure_elf(game_t *, wizard_t *, point_t);
static monster_t * conjure_spectre(game_t *, wizard_t *, point_t);
static monster_t * conjure_dire_wolf(game_t *, wizard_t *, point_t);
static monster_t * conjure_lion(game_t *, wizard_t *, point_t);
static monster_t * conjure_faun(game_t *, wizard_t *, point_t);
static monster_t * conjure_horse(game_t *, wizard_t *, point_t);
static monster_t * conjure_eagle(game_t *, wizard_t *, point_t);
static monster_t * conjure_vampire(game_t *, wizard_t *, point_t);


void chaos_conjure(spell_conjure_type_t type, game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster;

    switch(type)
    {
        case spell_conjure_type_bat:
            monster = conjure_bat(game, wizard, target);
            break;

        case spell_conjure_type_bear:
            monster = conjure_bear(game, wizard, target);
            break;

        case spell_conjure_type_ghost:
            monster = conjure_ghost(game, wizard, target);
            break;

        case spell_conjure_type_zombie:
            monster = conjure_zombie(game, wizard, target);
            break;

        case spell_conjure_type_dragon_gold:
            monster = conjure_golden_dragon(game, wizard, target);
            break;

        case spell_conjure_type_centaur:
            monster = conjure_centaur(game, wizard, target);
            break;

        case spell_conjure_type_dragon_green:
            monster = conjure_green_dragon(game, wizard, target);
            break;

        case spell_conjure_type_hydra:
            monster = conjure_hydra(game, wizard, target);
            break;

        case spell_conjure_type_skeleton:
            monster = conjure_skeleton(game, wizard, target);
            break;

        case spell_conjure_type_ogre:
            monster = conjure_ogre(game, wizard, target);
            break;

        case spell_conjure_type_goblin:
            monster = conjure_goblin(game, wizard, target);
            break;

        case spell_conjure_type_king_cobra:
            monster = conjure_king_cobra(game, wizard, target);
            break;

        case spell_conjure_type_wraith:
            monster = conjure_wraith(game, wizard, target);
            break;

        case spell_conjure_type_harpy:
            monster = conjure_harpy(game, wizard, target);
            break;

        case spell_conjure_type_giant_rat:
            monster = conjure_giant_rat(game, wizard, target);
            break;

        case spell_conjure_type_unicorn:
            monster = conjure_unicorn(game, wizard, target);
            break;

        case spell_conjure_type_orc:
            monster = conjure_orc(game, wizard, target);
            break;

        case spell_conjure_type_giant:
            monster = conjure_giant(game, wizard, target);
            break;

        case spell_conjure_type_gryphon:
            monster = conjure_gryphon(game, wizard, target);
            break;

        case spell_conjure_type_pegasus:
            monster = conjure_pegasus(game, wizard, target);
            break;

        case spell_conjure_type_gorilla:
            monster = conjure_gorilla(game, wizard, target);
            break;

        case spell_conjure_type_manticore:
            monster = conjure_manticore(game, wizard, target);
            break;

        case spell_conjure_type_dragon_red:
            monster = conjure_red_dragon(game, wizard, target);
            break;

        case spell_conjure_type_elf:
            monster = conjure_elf(game, wizard, target);
            break;

        case spell_conjure_type_dire_wolf:
            monster = conjure_dire_wolf(game, wizard, target);
            break;

        case spell_conjure_type_spectre:
            monster = conjure_spectre(game, wizard, target);
            break;

        case spell_conjure_type_lion:
            monster = conjure_lion(game, wizard, target);
            break;

        case spell_conjure_type_faun:
            monster = conjure_faun(game, wizard, target);
            break;

        case spell_conjure_type_horse:
            monster = conjure_horse(game, wizard, target);
            break;

        case spell_conjure_type_eagle:
            monster = conjure_eagle(game, wizard, target);
            break;

        case spell_conjure_type_vampire:
            monster = conjure_vampire(game, wizard, target);
            break;

        case spell_conjure_type_gooey_blob:
        case spell_conjure_type_magic_fire:

        default:
            break;
    }

    if(monster != NULL)
    {
        monster->owner = wizard;
    }

    return;
}

static monster_t * get_free_monster(game_t * game)
{
    for(int i = 0; i < MAX_MONSTERS_PER_GAME; i += 1)
    {
        if(game->monsters[i].is_free)
            return &game->monsters[i];
    }

    return NULL;
}

static monster_t * conjure_bat(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'b';
    monster->name = "Bat";
    monster->move_type = move_type_flying;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 5;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 1;
    monster->stat_defence = 1;
    monster->stat_manoeuvre_rating = 9;
    monster->stat_magic_resistance = 4;

    return monster;
}

static monster_t * conjure_bear(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'B';
    monster->name = "Bear";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 2;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 6;
    monster->stat_defence = 7;
    monster->stat_manoeuvre_rating = 6;
    monster->stat_magic_resistance = 2;

    return monster;
}

static monster_t * conjure_ghost(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'G';
    monster->name = "Ghost";
    monster->move_type = move_type_flying;
    monster->monster_type = monster_type_undead;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 2;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 1;
    monster->stat_defence = 3;
    monster->stat_manoeuvre_rating = 9;
    monster->stat_magic_resistance = 6;

    return monster;
}

static monster_t * conjure_zombie(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'Z';
    monster->name = "Zombie";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_undead;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 1;
    monster->stat_defence = 1;
    monster->stat_manoeuvre_rating = 2;
    monster->stat_magic_resistance = 3;

    return monster;
}

static monster_t * conjure_golden_dragon(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'G';
    monster->name = "Golden Dragon";
    monster->move_type = move_type_flying;
    monster->monster_type = monster_type_rideable;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 3;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_fire;
    monster->stat_range = 4;
    monster->stat_ranged_combat = 5;

    monster->stat_combat = 9;
    monster->stat_defence = 9;
    monster->stat_manoeuvre_rating = 5;
    monster->stat_magic_resistance = 5;

    return monster;
}

static monster_t * conjure_centaur(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'C';
    monster->name = "Centaur";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_rideable;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 4;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_bow;
    monster->stat_range = 4;
    monster->stat_ranged_combat = 2;

    monster->stat_combat = 1;
    monster->stat_defence = 3;
    monster->stat_manoeuvre_rating = 5;
    monster->stat_magic_resistance = 5;

    return monster;
}

static monster_t * conjure_green_dragon(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'g';
    monster->name = "Green Dragon";
    monster->move_type = move_type_flying;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 3;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_fire;
    monster->stat_range = 6;
    monster->stat_ranged_combat = 4;

    monster->stat_combat = 5;
    monster->stat_defence = 8;
    monster->stat_manoeuvre_rating = 4;
    monster->stat_magic_resistance = 4;

    return monster;
}

static monster_t * conjure_hydra(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'H';
    monster->name = "Hydra";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 7;
    monster->stat_defence = 8;
    monster->stat_manoeuvre_rating = 4;
    monster->stat_magic_resistance = 6;

    return monster;
}

static monster_t * conjure_skeleton(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'S';
    monster->name = "Skeleton";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_undead;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 3;
    monster->stat_defence = 2;
    monster->stat_manoeuvre_rating = 3;
    monster->stat_magic_resistance = 4;

    return monster;
}

static monster_t * conjure_ogre(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'O';
    monster->name = "Ogre";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 4;
    monster->stat_defence = 7;
    monster->stat_manoeuvre_rating = 3;
    monster->stat_magic_resistance = 6;

    return monster;
}

static monster_t * conjure_goblin(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'g';
    monster->name = "Goblin";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 2;
    monster->stat_defence = 4;
    monster->stat_manoeuvre_rating = 4;
    monster->stat_magic_resistance = 4;

    return monster;
}

static monster_t * conjure_king_cobra(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'K';
    monster->name = "King cobra";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 4;
    monster->stat_defence = 1;
    monster->stat_manoeuvre_rating = 6;
    monster->stat_magic_resistance = 1;

    return monster;
}

static monster_t * conjure_wraith(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'w';
    monster->name = "Wraith";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_undead;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 2;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 5;
    monster->stat_defence = 5;
    monster->stat_manoeuvre_rating = 4;
    monster->stat_magic_resistance = 5;

    return monster;
}

static monster_t * conjure_harpy(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'h';
    monster->name = "Harpy";
    monster->move_type = move_type_flying;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 5;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 4;
    monster->stat_defence = 2;
    monster->stat_manoeuvre_rating = 8;
    monster->stat_magic_resistance = 5;

    return monster;
}

static monster_t * conjure_giant_rat(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'r';
    monster->name = "Giant rat";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 3;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 1;
    monster->stat_defence = 1;
    monster->stat_manoeuvre_rating = 8;
    monster->stat_magic_resistance = 2;

    return monster;
}

static monster_t * conjure_unicorn(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'U';
    monster->name = "Unicorn";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 4;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 5;
    monster->stat_defence = 3;
    monster->stat_manoeuvre_rating = 9;
    monster->stat_magic_resistance = 7;

    return monster;
}

static monster_t * conjure_orc(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'o';
    monster->name = "Orc";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 2;
    monster->stat_defence = 1;
    monster->stat_manoeuvre_rating = 4;
    monster->stat_magic_resistance = 4;

    return monster;
}

static monster_t * conjure_giant(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'I';
    monster->name = "Giant";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 2;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 9;
    monster->stat_defence = 7;
    monster->stat_manoeuvre_rating = 6;
    monster->stat_magic_resistance = 5;

    return monster;
}

static monster_t * conjure_gryphon(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'R';
    monster->name = "Gryphon";
    monster->move_type = move_type_flying;
    monster->monster_type = monster_type_normal | monster_type_rideable;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 5;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 3;
    monster->stat_defence = 5;
    monster->stat_manoeuvre_rating = 5;
    monster->stat_magic_resistance = 6;

    return monster;
}

static monster_t * conjure_pegasus(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'R';
    monster->name = "Gryphon";
    monster->move_type = move_type_flying;
    monster->monster_type = monster_type_normal | monster_type_rideable;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 5;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 2;
    monster->stat_defence = 4;
    monster->stat_manoeuvre_rating = 6;
    monster->stat_magic_resistance = 7;

    return monster;
}

static monster_t * conjure_gorilla(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'O';
    monster->name = "Gorilla";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 6;
    monster->stat_defence = 5;
    monster->stat_manoeuvre_rating = 4;
    monster->stat_magic_resistance = 2;

    return monster;
}

static monster_t * conjure_red_dragon(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'R';
    monster->name = "Red Dragon";
    monster->move_type = move_type_flying;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 3;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_fire;
    monster->stat_range = 5;
    monster->stat_ranged_combat = 3;

    monster->stat_combat = 7;
    monster->stat_defence = 9;
    monster->stat_manoeuvre_rating = 4;
    monster->stat_magic_resistance = 5;

    return monster;
}

static monster_t * conjure_elf(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'E';
    monster->name = "Elf";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_bow;
    monster->stat_range = 6;
    monster->stat_ranged_combat = 2;

    monster->stat_combat = 1;
    monster->stat_defence = 2;
    monster->stat_manoeuvre_rating = 5;
    monster->stat_magic_resistance = 7;

    return monster;
}

static monster_t * conjure_manticore(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'M';
    monster->name = "Manticore";
    monster->move_type = move_type_flying;
    monster->monster_type = monster_type_normal | monster_type_rideable;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 5;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_fire;
    monster->stat_range = 3;
    monster->stat_ranged_combat = 1;

    monster->stat_combat = 3;
    monster->stat_defence = 6;
    monster->stat_manoeuvre_rating = 6;
    monster->stat_magic_resistance = 8;

    return monster;
}

static monster_t * conjure_spectre(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 's';
    monster->name = "Spectre";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_undead;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 4;
    monster->stat_defence = 2;
    monster->stat_manoeuvre_rating = 6;
    monster->stat_magic_resistance = 4;

    return monster;
}

static monster_t * conjure_dire_wolf(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'd';
    monster->name = "Dire Wolf";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 3;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 3;
    monster->stat_defence = 2;
    monster->stat_manoeuvre_rating = 7;
    monster->stat_magic_resistance = 2;

    return monster;
}

static monster_t * conjure_lion(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'L';
    monster->name = "Lion";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 4;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 6;
    monster->stat_defence = 4;
    monster->stat_manoeuvre_rating = 8;
    monster->stat_magic_resistance = 3;

    return monster;
}

static monster_t * conjure_faun(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'F';
    monster->name = "Faun";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 1;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 3;
    monster->stat_defence = 2;
    monster->stat_manoeuvre_rating = 7;
    monster->stat_magic_resistance = 8;

    return monster;
}

static monster_t * conjure_horse(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'H';
    monster->name = "Horse";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_rideable;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 4;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 1;
    monster->stat_defence = 3;
    monster->stat_manoeuvre_rating = 8;
    monster->stat_magic_resistance = 1;

    return monster;
}

static monster_t * conjure_eagle(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'E';
    monster->name = "Eagle";
    monster->move_type = move_type_flying;
    monster->monster_type = monster_type_normal;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 6;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 3;
    monster->stat_defence = 3;
    monster->stat_manoeuvre_rating = 8;
    monster->stat_magic_resistance = 2;

    return monster;
}

static monster_t * conjure_vampire(game_t * game, wizard_t * wizard, point_t target)
{
    monster_t * monster = get_free_monster(game);
    if(monster == NULL)
        return NULL;

    monster->color = wizard->color;
    monster->gfx = 'V';
    monster->name = "Vampire";
    monster->move_type = move_type_walking;
    monster->monster_type = monster_type_undead;

    monster->position = target;
    monster->owner = wizard;
    monster->is_illusion = wizard->selected_spell->is_illusion;
    monster->is_dead = false;
    monster->is_free = false;

    monster->moves_per_round = 4;
    monster->moves_left = monster->moves_per_round;

    monster->ranged_type = ranged_type_none;
    monster->stat_range = 0;
    monster->stat_ranged_combat = 0;

    monster->stat_combat = 6;
    monster->stat_defence = 8;
    monster->stat_manoeuvre_rating = 6;
    monster->stat_magic_resistance = 5;

    return monster;
}
