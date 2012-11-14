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
			if (move_mob(player, 0, -1))
				goto turn_done;

			print_msg("You cannot go there.");
			continue;

		case KEY_RIGHT:
			if (move_mob(player, 0, +1))
				goto turn_done;

			print_msg("You cannot go there.");
			continue;

		case KEY_UP:
			if (move_mob(player, -1, 0))
				goto turn_done;

			print_msg("You cannot go there.");
			continue;

		case KEY_DOWN:
			if (move_mob(player, +1, 0))
				goto turn_done;

			print_msg("You cannot go there.");
			continue;

		default:
			continue;
		}
	}

turn_done:
    explore();
    
	return 0;
}



void explore()
{
    int p_y;
    int p_x;

    int y;
    int x;

    p_y = mob[0].y;
    p_x = mob[0].x;

    level.map[p_y][p_x].explored = 1;
}
