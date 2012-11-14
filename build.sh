#!/bin/bash

<<<<<<< HEAD
gcc smallrl.c ui.c game.c level.c player.c -lm -lcurses -Wshadow -Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -std=c99 -pedantic -Wredundant-decls -ggdb -o smallrl
=======
FILES=(
    "level.c"
    "player.c"
    "smallrl.c"
    "ui.c"
    "game.c"
)

gcc -lm -lcurses -Wshadow -Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -std=c99 -pedantic -Wredundant-decls -ggdb -o smallrl ${FILES[*]}
>>>>>>> 60d21137971a5f8bd27958b953f8011c172578fe
