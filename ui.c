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
            if (level.map[y][x].explored == 0)
            {
                addch(' ');
                continue;
            }

            attrset(A_NORMAL);

            if (level.map[y][x].lit)
                attrset(A_BOLD);

            switch (level.map[y][x].type)
            {
            case tile_void:
                addch(' ');
                break;

            case tile_floor:
                attron(COLOR_PAIR(color_green));
                addch('.');
                break;

            case tile_corridor:
                addch('#');
                break;

            case tile_wall_r:
            case tile_wall_l:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_VLINE);
                break;

            case tile_wall_t:
            case tile_wall_b:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_HLINE);
                break;

            case tile_wall_ll:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_LLCORNER);
                break;

            case tile_wall_lr:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_LRCORNER);
                break;

            case tile_wall_ul:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_ULCORNER);
                break;

            case tile_wall_ur:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_URCORNER);
                break;

            case tile_stair:
                attron(A_NORMAL);
                addch('>');
                break;

            default:
                attron(COLOR_PAIR(color_red));
                addch('?');
                break;
            }
        }
    }

    attrset(A_NORMAL);

    for (i = 0; i < MOBS; i++)
    {
        if (mob[i].type && level.map[mob[i].y][mob[i].x].lit)
        {
            move(mob[i].y + 1, mob[i].x);
            addch(mob[i].type);
        }
    }

    attrset(A_NORMAL);

    refresh();

    return;
}



void draw_stats(void)
{
    move(22, 0);
    printw("HP: %d   Dlvl: %d", mob[0].hp, current_depth);
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
