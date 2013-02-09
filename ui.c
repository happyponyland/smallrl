#include <curses.h>

#include "ui.h"

#include "level.h"
#include "player.h"

void draw_map(level_t * level)
{
    int y;
    int x;
    int i;

    int draw_y = 1;

    for (y = level->view.ul_position.y; y < level->view.ul_position.y + level->view.height; y += 1)
    {
        move(++draw_y, 0);

        for (x = level->view.ul_position.x; x < level->view.ul_position.x + level->view.width; x += 1)
        {
            if(!on_map(level, y, x))
               continue;

            if (level->map[y * level->width + x].is_explored == 0)
            {
                addch(' ');
                continue;
            }

            attrset(A_NORMAL);

            if (level->map[y * level->width + x].item)
            {
                addch('/');
                continue;
            }

            if (level->map[y * level->width + x].is_lit ||
                level->map[y * level->width + x].is_periphery)
            {
                attrset(A_BOLD);
            }

            switch (level->map[y * level->width + x].type)
            {
            case tile_void:
                addch(' ');
                break;

            case tile_floor:
                attron(COLOR_PAIR(color_green));
                addch('.');
                break;

            case tile_doorway:
                attron(COLOR_PAIR(color_green));
                addch(ACS_BLOCK);
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

            case tile_wall_plus:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_PLUS);
                break;

            case tile_wall_ttee:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_TTEE);
                break;

            case tile_wall_btee:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_BTEE);
                break;

            case tile_wall_ltee:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_LTEE);
                break;

            case tile_wall_rtee:
                attron(COLOR_PAIR(color_yellow));
                addch(ACS_RTEE);
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

    for (i = 0; i < MAX_MOBS_PER_LEVEL; i++)
    {
        if (level->mobs[i].type && level->map[level->mobs[i].position.y * level->width + level->mobs[i].position.x].is_lit)
        {
            int left = level->view.ul_position.x;
            int top = level->view.ul_position.y;

            int right  = level->view.ul_position.x + level->view.width;
            int bottom  = level->view.ul_position.y + level->view.height;

            int mob_x = level->mobs[i].position.x;
            int mob_y = level->mobs[i].position.y;

            if(mob_x > left && mob_x < right && mob_y > top && mob_y < bottom)
            {
                move(level->mobs[i].position.y - top + 2, level->mobs[i].position.x - left);
                addch(level->mobs[i].type);
            }
        }
    }

    attrset(A_NORMAL);

    refresh();

    return;
}



void draw_stats()
{
    move(22, 0);
    printw("HP: %d    Damage: %d-%d",
           player.mob->attr[ATTR_HP],
           player.mob->attr[ATTR_MINDAM],
           player.mob->attr[ATTR_MINDAM] + player.mob->attr[ATTR_MAXDAM]);
    clrtoeol();

    move(23, 0);
    printw("LVL: %d (%ld TNL)  DLVL: %d",
           player.level, get_player_tnl(),
           current_level->depth);
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
