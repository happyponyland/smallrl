#include <curses.h>
#include <stdlib.h>

#include "smallrl.h"


int main(int argc, char ** argv)
{
  int i;

  initscr();

  if (LINES < 24 || COLS < 80)
  {
    shutdown();
    printf("Terminal too small.");
    exit(1);
  }

  new_game();

  shutdown();
  
  return 0;
}



void draw_map()
{
  int y;
  int x;
  int i;

  for (y = 0; y < MAP_H; y++)
    for (x = 0; x < MAP_W; x++)
      map[y][x] = tile_floor;

  for (i = 1; i < MOBS; i++)
    mob[i].type = MOB_NONE;

  return;
}



void new_game()
{
  clear_map();
}

 
void clear_map()
{
  int y;
  int x;
  int i;

  for (i = 1; i < MOBS; i++)
    mob[i].type = MOB_NONE;

  for (y = 0; y < MAP_H; y++)
    for (x = 0; x < MAP_W; x++)
      map[y][x] = tile_floor;
}


void shutdown()
{
  endwin();
  return;
}
