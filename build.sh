#!/bin/bash

FILES=(
    "level.c"
    "player.c"
    "smallrl.c"
    "ui.c"
    "game.c"
    "ai.c"
    "boring.c"
    "combat.c"
    "mob.c"
)

gcc -lm -lcurses -Wshadow -Wall -Wstrict-prototypes \
    -Wmissing-prototypes -Wmissing-declarations -Wnested-externs \
    -std=c99 -pedantic -Wredundant-decls -ggdb \
    -o smallrl ${FILES[*]}
