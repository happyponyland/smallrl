#include <stdlib.h>
#include <curses.h>
#include <time.h>

#include "game.h"
#include "ui.h"

static void shutdown(void);

int main(int argc, char ** argv)
{
    time_t random_seed = time(NULL);

    if(argc == 2)
        random_seed = strtoul(argv[1], NULL, 10);

    printf("Random game #%ld\n", random_seed);
    srand(random_seed);

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(color_black,   COLOR_BLACK,   COLOR_BLACK);
    init_pair(color_white,   COLOR_WHITE,   COLOR_BLACK);
    init_pair(color_yellow,  COLOR_YELLOW,  COLOR_BLACK);
    init_pair(color_blue,    COLOR_BLUE,    COLOR_BLACK);
    init_pair(color_red,     COLOR_RED,     COLOR_BLACK);
    init_pair(color_green,   COLOR_GREEN,   COLOR_BLACK);
    init_pair(color_cyan,    COLOR_CYAN,    COLOR_BLACK);
    init_pair(color_magenta, COLOR_MAGENTA, COLOR_BLACK);


    if (LINES < 24 || COLS < 80)
    {
        shutdown();
        printf("Terminal too small.");
        exit(1);
    }

    do
    {
      erase();
      new_game();
    }
    while (play());

    shutdown();

    return 0;
}


static void shutdown(void)
{
    endwin();
    return;
}
