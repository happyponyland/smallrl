#!/bin/bash

FILES=(
    "level.c"
    "player.c"
    "smallrl.c"
    "ui.c"
    "game.c"
    "item.c"
    "ai.c"
    "standard_dungeon.c"
    "standard_dungeon_internal_item_tables.c"
    "combat.c"
    "mob.c"
    "los.c"
    "chaos.c"
    "chaos_internal_ui.c"
    "chaos_internal_ai.c"
    "chaos_internal_conjure.c"
)

gcc -lm -lcurses -Wshadow -Wall -Wextra -Wstrict-prototypes \
    -Wmissing-prototypes -Wmissing-declarations -Wnested-externs \
    -std=c99 -pedantic -Wredundant-decls -ggdb -pg \
    -o smallrl ${FILES[*]}
