#include "smallrl.h"


void enemy_tick(mob_t * enemy)
{
	move_mob(enemy, -1 + rand() % 3, -1 + rand() % 3);
	enemy->next_think += TIMESTEP * 10;
}


int pathfind(int id, int dest_y, int dest_x, int * y_speed, int * x_speed)
{

	return 0;
}
