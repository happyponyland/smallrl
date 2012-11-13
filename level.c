#include "smallrl.h"


int on_map(int y, int x)
{
  if (y >= 0 && x >= 0 && y < MAP_H && x < MAP_W)
    return 1;

  return 0;
}


void clear_map()
{
  int y;
  int x;
  int i;

  for (i = 1; i < MOBS; i++)
    mob[i].type = mob_none;

  for (y = 0; y < MAP_H; y++)
    for (x = 0; x < MAP_W; x++)
      level.map[y][x] = tile_floor;

  return;
}

