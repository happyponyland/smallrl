#include "smallrl.h"


int player_turn()
{
    int input;

    while (1)
    {
        input = getch();

        switch (input)
        {
        case 'Q':
            if (prompt_yn("Do you want to quit?"))
                return 1;

            continue;

        case ' ':
            clear_msg();
            continue;

        case KEY_LEFT:
            if (move_mob(0, 0, -1))
                goto turn_done;

            print_msg("You cannot go there.");
            continue;

        case KEY_RIGHT:
            if (move_mob(0, 0, +1))
                goto turn_done;

            print_msg("You cannot go there.");
            continue;

        case KEY_UP:
            if (move_mob(0, -1, 0))
                goto turn_done;

            print_msg("You cannot go there.");
            continue;

        case KEY_DOWN:
            if (move_mob(0, +1, 0))
                goto turn_done;

            print_msg("You cannot go there.");
            continue;

        default:
            continue;
        }
    }

turn_done:
    return 0;
}
