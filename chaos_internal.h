#ifndef SMALLRL_CHAOS_INTERNAL_H
#define SMALLRL_CHAOS_INTERNAL_H

#define BOARD_WIDTH (15)
#define BOARD_HEIGHT (10)

#define MAX_WIZARDS_PER_GAME (8)
#define MAX_SPELLS_PER_WIZARD (50)
#define MAX_MONSTERS_PER_GAME (128)

#include <stdbool.h>
#include "colors.h"

typedef enum cursor_action_e
{
    cursor_action_move_up,
    cursor_action_move_down,
    cursor_action_move_left,
    cursor_action_move_right,
    cursor_action_fire
} cursor_action_t;

typedef enum spell_order_e
{
    spell_order_chaos,
    spell_order_law,
    spell_order_neutral
} spell_order_t;

typedef enum spell_type_e
{
    spell_type_conjure, /* Normal monster conjuring */
    spell_type_transform, /* Wizard modifiers, knife, sword, shadow form, wings */
    spell_type_projectile, /* Magic bolt, lightning, spells */
    spell_type_object, /* Static conjuring (trees / castles) */
    spell_type_global /* No specific target */
} spell_type_t;

typedef enum spell_conjure_type_e
{
    spell_conjure_type_gooey_blob,
    spell_conjure_type_magic_fire,
    spell_conjure_type_bat,
    spell_conjure_type_bear,
    spell_conjure_type_centaur,
    spell_conjure_type_dire_wolf,
    spell_conjure_type_dragon_red,
    spell_conjure_type_dragon_gold,
    spell_conjure_type_dragon_green,
    spell_conjure_type_eagle,
    spell_conjure_type_elf,
    spell_conjure_type_faun,
    spell_conjure_type_ghost,
    spell_conjure_type_giant,
    spell_conjure_type_giant_rat,
    spell_conjure_type_goblin,
    spell_conjure_type_gorilla,
    spell_conjure_type_gryphon,
    spell_conjure_type_harpy,
    spell_conjure_type_horse,
    spell_conjure_type_hydra,
    spell_conjure_type_king_cobra,
    spell_conjure_type_lion,
    spell_conjure_type_manticore,
    spell_conjure_type_ogre,
    spell_conjure_type_orc,
    spell_conjure_type_pegasus,
    spell_conjure_type_skeleton,
    spell_conjure_type_spectre,
    spell_conjure_type_unicorn,
    spell_conjure_type_vampire,
    spell_conjure_type_wraith,
    spell_conjure_type_zombie
} spell_conjure_type_t;

typedef enum spell_projectile_type_e
{
    spell_projectile_type_disbelieve,
    spell_projectile_type_bolt,
    spell_projectile_type_lightning,
    spell_projectile_type_vengeance, /* 1 attack on enemy creature */
    spell_projectile_type_justice, /* 3 attacks on wizard(s), removes all owned by wizard */
    spell_projectile_type_dark_power, /* 3 attacks on enemy creatures */
    spell_projectile_type_decree, /* 1 attack on enemy creature */
    spell_projectile_type_raise_dead,
    spell_projectile_type_subversion
} spell_projectile_type_t;

typedef enum spell_transform_type_e
{
    spell_transform_type_none = 0,
    spell_transform_type_wings = 1,
    spell_transform_type_knife = 2,
    spell_transform_type_shadow_form = 4,
    spell_transform_type_sword = 8,
    spell_transform_type_armour = 16,
    spell_transform_type_bow = 32
} spell_transform_type_t;

typedef enum spell_object_type_e
{
    spell_object_type_wall,
    spell_object_type_magic_castle,
    spell_object_type_dark_citadel,
    spell_object_type_shadow_wood,
    spell_object_type_magic_wood
} spell_object_type_t;

typedef enum spell_global_type_e
{
    spell_global_type_law,
    spell_global_type_chaos,
    spell_global_type_turmoil,
} spell_global_type_t;

typedef struct spell_s
{
    spell_order_t order;
    char * name;
    int range;
    spell_type_t type;
    bool is_stoppable;
    int subtype; // Enum
    int difficulty;

    // collection
    bool is_free;
} spell_t;

typedef struct spell_list_s
{
    spell_t * current;
    spell_t * previous;
    spell_t * next;
} spell_list_t;

typedef struct point_s
{
    int x;
    int y;
} point_t;

typedef struct wizard_s
{
    point_t position;
    color_t color;
    char gfx;
    char * name;
    spell_t spells[MAX_SPELLS_PER_WIZARD];
    spell_t * selected_spell;
    spell_transform_type_t active_transformations;
    int skill;
    spell_order_t order;
    bool is_alive;
    bool is_computer;

    // Collection
    bool is_free;
} wizard_t;

typedef enum monster_type_e
{
    monster_type_unknown = 0,
    monster_type_undead = 1,
    monster_type_flying = 2,
    monster_type_rideable = 4
} monster_type_t;

typedef enum ranged_type_e
{
    ranged_type_none,
    ranged_type_bow,
    ranged_type_fire
} ranged_type_t;

typedef struct monster_s
{
    wizard_t * owner;
    point_t position;
    color_t color;
    char gfx;
    char * name;
    monster_type_t monster_type;
    bool is_illusion;
    ranged_type_t ranged_type;
    bool is_dead;

    // Collection
    bool is_free;
} monster_t;

typedef struct game_s
{
    wizard_t wizards[MAX_WIZARDS_PER_GAME];
    monster_t monsters[MAX_MONSTERS_PER_GAME];
    point_t board_offset;
} game_t;

#endif
