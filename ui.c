#include "smallrl.h"


void draw_map()
{
  int y;
  int x;
  int i;

  for (y = 0; y < MAP_H; y++)
  {
    move(y + 1, 0);

    for (x = 0; x < MAP_W; x++)
    {
      switch (level.map[y][x])
      {
	case tile_void:
	  addch(' ');
	  break;

	case tile_floor:
	  addch('.');
	  break;

	case tile_wall:
	  addch('#');
	  break;

	case tile_stair:
	  addch('>');
	  break;

	default:
	  addch('?');
	  break;
      }
    }
  }
    
  for (i = 0; i < MOBS; i++)
  {
    if (mob[i].type)
    {
      move(mob[i].y + 1, mob[i].x);
      addch(mob[i].type);
    }
  }

  refresh();

  return;
}



void draw_stats(void)
{
  move(22, 0);
  printw("HP: %d", mob[0].hp);
  clrtoeol();

  refresh();

  return;
}



int prompt_yn(char * m)
{
  print_msg(m);

  if (getch() == 'y')
  {
    clear_msg();
    return 1;
  }
  else
  {
    clear_msg();
    return 0;
  }
}



void wait()
{
  move(0, 75);

  attron(A_REVERSE);
  addstr(" MORE");
  attroff(A_REVERSE);
  refresh();
  
  getch();

  return;
}


void print_msg(char * s)
{
  move(0, 0);
  addstr(s);
  clrtoeol();
  refresh();

  return;
}



void clear_msg()
{
  move(0, 0);
  clrtoeol();
  refresh();

  return;
}


