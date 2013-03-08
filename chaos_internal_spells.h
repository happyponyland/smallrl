#ifndef SMALLRL_CHAOS_SPELLS_H
#define SMALLRL_CHAOS_SPELLS_H

#include "chaos_internal.h"

spell_t chaos_spell_disbelieve = {
    .order = spell_order_neutral,
    .name = "Disbelieve",
    .range = 20,
    .type = spell_type_projectile,
    .is_stoppable = false,
    .subtype = (int)spell_projectile_type_disbelieve,
    .difficulty = 0,
    .is_free = false,
};

spell_t chaos_spells[5] = {
    {
        .order = spell_order_law,
        .name = "Bat",
        .range = 1,
        .type = spell_type_conjure,
        .is_stoppable = true,
        .subtype = (int)spell_conjure_type_bat,
        .difficulty = 2,
        .is_free = false,
    },
    {
        .order = spell_order_law,
        .name = "Bat",
        .range = 1,
        .type = spell_type_conjure,
        .is_stoppable = true,
        .subtype = (int)spell_conjure_type_bat,
        .difficulty = 2,
        .is_free = false,
    },
    {
        .order = spell_order_law,
        .name = "Lion",
        .range = 1,
        .type = spell_type_conjure,
        .is_stoppable = true,
        .subtype = (int)spell_conjure_type_lion,
        .difficulty = 3,
        .is_free = false
    },
    {
        .order = spell_order_law,
        .name = "Elf",
        .range = 1,
        .type = spell_type_conjure,
        .is_stoppable = true,
        .subtype = (int)spell_conjure_type_elf,
        .difficulty = 3,
        .is_free = false,
    },
    {
        .order = spell_order_law,
        .name = "Eagle",
        .range = 1,
        .type = spell_type_conjure,
        .is_stoppable = true,
        .subtype = (int)spell_conjure_type_eagle,
        .difficulty = 3,
        .is_free = false,
    }
};

#endif
