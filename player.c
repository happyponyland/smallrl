#include "smallrl.h"



int player_turn()
{
	int input;

	while (1)
	{
		input = getch();

        clear_msg();

		switch (input)
		{
		case 'Q':
			if (prompt_yn("Do you want to quit?"))
				return 1;

			continue;

		case ' ':
			//clear_msg();
			continue;

		case KEY_LEFT:
			if (move_mob(player, 0, -1))
            {
                explore();
				goto turn_done;
            }

			print_msg("You cannot go there.");
			continue;

		case KEY_RIGHT:
			if (move_mob(player, 0, +1))
            {
                explore();
				goto turn_done;
            }

			print_msg("You cannot go there.");
			continue;

		case KEY_UP:
			if (move_mob(player, -1, 0))
            {
                explore();
				goto turn_done;
            }

			print_msg("You cannot go there.");
			continue;

		case KEY_DOWN:
			if (move_mob(player, +1, 0))
            {
                explore();
				goto turn_done;
            }

			print_msg("You cannot go there.");
			continue;

		default:
			continue;
		}
	}

turn_done:
    
	return 0;
}



/*
  Lights up the area near the player, marks it as explored.
*/
void explore()
{
    int p_y;
    int p_x;

    int y;
    int x;

    p_y = mob[0].y;
    p_x = mob[0].x;

    for (y = 0; y < MAP_H; y++)
        for (x = 0; x < MAP_W; x++)
            level.map[y][x].lit = 0;

    for (y = p_y - 1; y <= p_y + 1; y++)
    {
        for (x = p_x - 1; x <= p_x + 1; x++)
        {
            if (on_map(y, x))
            {
                level.map[y][x].explored = 1;
                level.map[y][x].lit = 1;
            }
        }
    }

    /* Floodfill open permalit rooms */

    if (level.map[p_y][p_x].flags & tile_permalit)
    {
        int change;

        do
        {
            change = 0;
            
            for (y = 1; y < MAP_H - 1; y++)
            {
                for (x = 1; x < MAP_W - 1; x++)
                {
                    if (level.map[y][x].lit == 0 &&
                        (level.map[y][x].flags & tile_permalit ||
                         level.map[y - 1][x - 1].flags & tile_permalit ||
                         level.map[y - 1][x + 1].flags & tile_permalit ||
                         level.map[y + 1][x - 1].flags & tile_permalit ||
                         level.map[y + 1][x + 1].flags & tile_permalit ||
                         level.map[y - 1][x].flags & tile_permalit ||
                         level.map[y][x - 1].flags & tile_permalit ||
                         level.map[y + 1][x].flags & tile_permalit ||
                         level.map[y][x + 1].flags & tile_permalit ))
                    {
                        if (level.map[y - 1][x].lit ||
                            level.map[y][x - 1].lit ||
                            level.map[y + 1][x].lit ||
                            level.map[y][x + 1].lit)
                        {
                            level.map[y][x].lit = 1;
                            level.map[y][x].explored = 1;
                            
                            change = 1;
                        }
                        
                    }
                }
            }
        } while (change);
    }

    return;
}
