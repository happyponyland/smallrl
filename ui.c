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
                attrset(COLOR_PAIR(color_green));
                addch('.');
                break;

            case tile_corridor:
                attrset(A_NORMAL);
                addch('#');
                break;

            case tile_wall_r:
            case tile_wall_l:
                attrset(COLOR_PAIR(color_yellow));
                addch(ACS_VLINE);
                break;

            case tile_wall_t:
            case tile_wall_b:
                attrset(COLOR_PAIR(color_yellow));
                addch(ACS_HLINE);
                break;

            case tile_wall_ll:
                attrset(COLOR_PAIR(color_yellow));
                addch(ACS_LLCORNER);
                break;

            case tile_wall_lr:
                attrset(COLOR_PAIR(color_yellow));
                addch(ACS_LRCORNER);
                break;

            case tile_wall_ul:
                attrset(COLOR_PAIR(color_yellow));
                addch(ACS_ULCORNER);
                break;

            case tile_wall_ur:
                attrset(COLOR_PAIR(color_yellow));
                addch(ACS_URCORNER);
                break;

            case tile_stair:
                attrset(A_NORMAL);
                addch('>');
                break;

            default:
                attrset(COLOR_PAIR(color_red));
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
