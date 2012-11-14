#!/bin/sh

gcc smallrl.c ui.c game.c level.c player.c -lm -lcurses -Wshadow -Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -std=c99 -pedantic -Wredundant-decls -ggdb -o smallrl
