#include "smallrl.h"


void enemy_turn(int id)
{
    int x_speed;
    int y_speed;

}


int pathfind(int id, int dest_y, int dest_x, int * y_speed, int * x_speed)
{
    int y;
    int x;

    int change;

    if (!on_map(mob[id].y, mob[id].x) ||
        x_speed == NULL || y_speed == NULL)
    {
        return -1;
    }

    for (y = 0; y < MAP_H; y++)
    {
        for (x = 0; x < MAP_W; x++)
        {
            level.map[y][x].path = 99999;
        }
    }

    do
    {
        change = 0;

        for (y = 0; y < MAP_H; y++)
        {
            for (x = 0; x < MAP_W; x++)
            {
                level.map[y][x].path = 99999;
            }
        }
    } while (change);

    return 0;
}
