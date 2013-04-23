#include <stdlib.h>

#include "level.h"
#include "player.h"
#include "combat.h"

level_t * levels;
level_t * current_level;

static void clear_level(level_t *);
static void set_tile_flags_by_type(tile_t *, tile_type_t);
static void describe_mob(mob_t *);

bool on_map(level_t * level, int y, int x)
{
    if (y >= 0 && x >= 0 && y < level->height && x < level->width)
        return true;

    return false;
}

level_t * create_new_level(level_t * old_level, int height, int width)
{
    level_t * new_level = malloc(sizeof(level_t));
    new_level->width = width;
    new_level->height = height;
    new_level->map = malloc(sizeof(tile_t) * width * height);
    new_level->view = (window_t) {.ul_position = (point_t) { .x = 0, .y = 0 }, .width = 80, .height = 20 };
    clear_level(new_level);

    new_level->mobs[0].type = mob_player;
    new_level->up_level = old_level;
    new_level->depth = old_level != NULL ? old_level->depth + 1 : 1;

    return new_level;
}

void center_view(level_t * level, point_t center_position)
{
    int x = center_position.x;
    int y = center_position.y;

    int half_window_width = level->view.width / 2;
    int half_window_height = level->view.height / 2;

    x -= half_window_width;
    if(x < 0)
        x = 0;

    y -= half_window_height;
    if(y < 0)
        y = 0;

    if(x > level->width - half_window_width * 2)
        x = level->width - half_window_width * 2;

    if(y > level->height - half_window_height * 2)
        y = level->height - half_window_height * 2;

    level->view.ul_position.x = x;
    level->view.ul_position.y = y;

    return;
}

static void clear_level(level_t * level)
{
    int y;
    int x;
    int i;

    for (i = 1; i < MAX_MOBS_PER_LEVEL; i++)
        level->mobs[i].type = mob_none;

    for (y = 0; y < level->height; y++)
        for (x = 0; x < level->width; x++) {
            set_tile(level, y, x, tile_void);
            level->map[y * level->width + x].is_explored = 0;
            level->map[y * level->width + x].is_lit = 0;
            level->map[y * level->width + x].item = 0;
        }

    return;
}


void set_tile(level_t * level, int y, int x, tile_type_t t)
{
    if (on_map(level, y, x)) {
        level->map[y * level->width + x].type = t;
        set_tile_flags_by_type(&level->map[y * level->width + x], t);
    }

    return;
}

static void set_tile_flags_by_type(tile_t * tile, tile_type_t tile_type)
{
    tile->flags = 0;

    switch(tile_type) {
        case tile_wall_t:
        case tile_wall_b:
        case tile_wall_r:
        case tile_wall_l:
        case tile_wall_ll:
        case tile_wall_lr:
        case tile_wall_ul:
        case tile_wall_ur:
        case tile_wall_ttee:
        case tile_wall_btee:
        case tile_wall_rtee:
        case tile_wall_ltee:
        case tile_wall_plus:
        case tile_void:
            tile->flags |= tile_unpassable;
            tile->flags |= tile_noflood;
            break;

        case tile_doorway:
            tile->flags |= tile_noflood;
            break;

        case tile_floor:
        case tile_stair:
            tile->flags |= tile_permalit;
            break;

        default:
            break;
    }
}

tile_type_t get_tile_type(level_t * level, int y, int x)
{
    if (on_map(level, y, x))
        return level->map[y * level->width + x].type;

    return tile_void;
}

bool try_move_mob(level_t * level, mob_t * mob_to_move, int y_speed, int x_speed)
{
    int new_y = mob_to_move->position.y + y_speed;
    int new_x = mob_to_move->position.x + x_speed;

    if (on_map(level, new_y, new_x) &&
        !(level->map[new_y * level->width + new_x].flags & tile_unpassable))
    {
        if(new_y == player.mob->position.y && new_x == player.mob->position.x)
        {
            attack(mob_to_move, player.mob);
            return 0;
        }

        for(int i = 1; i < MAX_MOBS_PER_LEVEL; ++i)
        {
            if(level->mobs[i].type == mob_none)
                continue;

            if(new_y == level->mobs[i].position.y && new_x == level->mobs[i].position.x)
                return 0;
        }

        mob_to_move->position.y = new_y;
        mob_to_move->position.x = new_x;

        return true;
    }

    return false;
}

static void describe_mob(mob_t * mob)
{
    mob_name(mob->type, mob->description_noun);
    mob_adjective(mob->type, mob->description_adjective);
}

bool try_make_mob(level_t * level, mob_type_t type, int y, int x)
{
    int i;

    for (i = 1; i < MAX_MOBS_PER_LEVEL; i++)
    {
        mob_t * mob = &level->mobs[i];
        if (mob->type == mob_none)
        {
            mob->position.y = y;
            mob->position.x = x;

            mob->type = type;
            mob->turn_counter = 0;
            mob->is_immortal = false;

            switch (type)
            {
            case mob_newbie:
                mob->attr[ATTR_HP] = 5;
                mob->attr[ATTR_MINDAM] = 1;
                mob->attr[ATTR_MAXDAM] = 3;
                mob->attr[ATTR_ATTACK] = 40;
                mob->attr[ATTR_DODGE] = 20;
                mob->attr[ATTR_EXP] = 30;
                break;

            case mob_magician:
                mob->is_immortal = true;
                mob->attr[ATTR_HP] = 1;
                mob->attr[ATTR_MINDAM] = 0;
                mob->attr[ATTR_MAXDAM] = 0;
                mob->attr[ATTR_ATTACK] = 0;
                mob->attr[ATTR_DODGE] = 0;
                mob->attr[ATTR_EXP] = 200;
                break;

            case mob_zombie:
                mob->attr[ATTR_HP] = 12;
                mob->attr[ATTR_MINDAM] = 2;
                mob->attr[ATTR_MAXDAM] = 5;
                mob->attr[ATTR_ATTACK] = 30;
                mob->attr[ATTR_DODGE] = 10;
                mob->attr[ATTR_EXP] = 45;
                break;

            case mob_1337hax0r:
                mob->attr[ATTR_HP] = 1;
                mob->attr[ATTR_MINDAM] = 1;
                mob->attr[ATTR_MAXDAM] = 0;
                mob->attr[ATTR_ATTACK] = 1;
                mob->attr[ATTR_DODGE] = 60;
                mob->attr[ATTR_EXP] = 15;
                break;

            case mob_hobbyist:
                mob->attr[ATTR_HP] = 5;
                mob->attr[ATTR_MINDAM] = 1;
                mob->attr[ATTR_MAXDAM] = 3;
                mob->attr[ATTR_ATTACK] = 30;
                mob->attr[ATTR_DODGE] = 50;
                mob->attr[ATTR_EXP] = 40;
                break;

            case mob_war_troll:
                mob->attr[ATTR_HP] = 20;
                mob->attr[ATTR_MINDAM] = 2;
                mob->attr[ATTR_MAXDAM] = 6;
                mob->attr[ATTR_ATTACK] = 20;
                mob->attr[ATTR_DODGE] = 10;
                mob->attr[ATTR_EXP] = 90;
                break;

            case mob_police:
                mob->attr[ATTR_HP] = 10;
                mob->attr[ATTR_MINDAM] = 2;
                mob->attr[ATTR_MAXDAM] = 4;
                mob->attr[ATTR_ATTACK] = 10;
                mob->attr[ATTR_DODGE] = 10;
                mob->attr[ATTR_EXP] = 20;
                break;

            default:
                mob->attr[ATTR_HP] = 5;
                break;
            }

            describe_mob(mob);

            return true;
        }
    }

    return false;
}



int get_mob(level_t * level, int y, int x)
{
    int i;

    for (i = 0; i < MAX_MOBS_PER_LEVEL; i++)
        if (level->mobs[i].type && level->mobs[i].position.y == y && level->mobs[i].position.x == x)
            return i;

    return -1;
}
