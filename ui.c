#include <curses.h>
#include <string.h>

#include "ui.h"

#include "level.h"
#include "player.h"

#include "log.h"

static bool cursor_blinker = true;

static void draw_log_border(input_type_t, level_t *);
static void draw_stats_border(void);
static void draw_map_border(input_type_t, level_t *);
static void draw_box(int, int, int, int);

static void draw_box(int x, int y, int width, int height)
{
    move(y, x);
    hline(ACS_HLINE, width);
    move(y, x + width);
    vline(ACS_VLINE, height);
    move(y, x);
    vline(ACS_VLINE, height);
    move(y + height, x);
    hline(ACS_HLINE, width);

    move(y, x);
    addch(ACS_ULCORNER);

    move(y, x + width);
    addch(ACS_URCORNER);

    move(y + height, x);
    addch(ACS_LLCORNER);

    move(y + height, x + width);
    addch(ACS_LRCORNER);
}

static void draw_map_border(input_type_t input_type, level_t * level)
{
    if(input_type == input_type_map)
    {
        attrset(A_BOLD);
    }
    else
    {
        attrset(A_NORMAL);
    }

    draw_box(0, 1, level->view.width + 1, level->view.height + 1);
}

static void draw_stats_border()
{
}

static void draw_log_border(input_type_t input_type, level_t * level)
{
    if(input_type == input_type_log)
    {
        attrset(A_BOLD);
    }
    else
    {
        attrset(A_NORMAL);
    }

    draw_box(level->view.width + 2, 1, 40, 23);
}

void draw_map(input_type_t input_type, level_t * level)
{
    int y;
    int x;
    int i;

    int offset_y = 1;
    int offset_x = 1;

    int draw_y = offset_y;

    draw_map_border(input_type, level);

    for (y = level->view.ul_position.y; y < level->view.ul_position.y + level->view.height; y += 1)
    {
        move(++draw_y, offset_x);

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
                move(level->mobs[i].position.y - top + offset_y + 1, level->mobs[i].position.x - left + offset_x);
                addch(level->mobs[i].type);
            }
        }
    }

    attrset(A_NORMAL);

    refresh();

    return;
}

void draw_stats(player_info_t * player, level_t * level)
{
    int offset = 3;

    draw_stats_border();

    move(level->view.height + offset++, 0);
    printw("HP: %d    Damage: %d-%d",
           player->mob->attr[ATTR_HP],
           player->mob->attr[ATTR_MINDAM],
           player->mob->attr[ATTR_MINDAM] + player->mob->attr[ATTR_MAXDAM]);
    clrtoeol();

    move(level->view.height + offset++, 0);
    printw("LVL: %d (%ld TNL)  DLVL: %d",
           player->level, get_player_tnl(player),
           level->depth);
    clrtoeol();

    refresh();

    return;
}

void draw_log(input_type_t input_type, log_t * log, level_t * level)
{
    int y;

    draw_log_border(input_type, level);

    for(y = 2; y < 23; y += 1)
    {
        move(y, level->view.width + 3);
        for(int x = 0; x < 39; x += 1)
            addch(' ');
    }

    move(23, level->view.width + 3);
    addch(' ');
    addch('>');
    addch(' ');

    addstr(log->log_input);

    for(int i = level->view.width + 4 + 2 + strlen(log->log_input); i < level->view.width + 42; i += 1) {
        addch(' ');
    }

    move(23, level->view.width + 4 + 2 + strlen(log->log_input));

    if(input_type == input_type_log) {
        if(cursor_blinker ^= true) {
            addch(ACS_CKBOARD);
        } else {
            addch(' ');
        }
    }

    draw_box(level->view.width + 2, 1, 40, 23);

    y = 21;
    int temp_index = log->message_log_index;
    while(y >= 2 && temp_index >= 0)
    {
        move(y, level->view.width + 4);
        addstr(log->message_log[temp_index]);

        y -= 1;
        temp_index -= 1;
    }

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
