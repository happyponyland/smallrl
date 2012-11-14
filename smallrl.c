#include "smallrl.h"

#include <time.h>


int main(int argc, char ** argv)
{
    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(color_black,   COLOR_BLACK,   COLOR_BLACK);
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





void shutdown()
{
    endwin();
    return;
}
