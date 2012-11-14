#include "smallrl.h"


int play()
{
    int i;

    while (1)
    {
        draw_map();
        draw_stats();

        if (mob[0].hp <= 0)
            return prompt_yn("You died. Play again?");

        if (player_turn())
            return 0;

        for (i = 1; i < MOBS; i++)
        {
            enemy_turn(i);
        }
    }

    return 0;
}




void enemy_turn(int id)
{
}



void new_game()
{
    //clear_map();
    boring_level(&mob[0].x, &mob[0].y);

    mob[0].type = mob_player;
    mob[0].hp = 20;

    return;
}



int move_mob(int id, int y_speed, int x_speed)
{
    int new_y = mob[id].y + y_speed;
    int new_x = mob[id].x + x_speed;

    if (on_map(new_y, new_x) &&
        level.map[new_y][new_x] != tile_wall_r)
    {
        mob[id].y = new_y;
        mob[id].x = new_x;

        return 1;
    }

    return 0;
}
