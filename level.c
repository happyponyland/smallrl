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
        for (x = 0; x < MAP_W; x++) {
            set_tile(y, x, tile_void);
            level.map[y][x].explored = 0;
            level.map[y][x].lit = 0;
        }

    return;
}


void set_tile(int y, int x, tile_type_t t)
{
    if (on_map(y, x)) {
        level.map[y][x].type = t;
        set_tile_flags_by_type(&level.map[y][x], t);
    }

    return;
}

void set_tile_flags_by_type(tile_t * tile, tile_type_t tile_type)
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
        case tile_void:
            tile->flags |= tile_unpassable;
            break;

        case tile_floor:
        case tile_stair:
            tile->flags |= tile_permalit;
            break;

        default:
            break;
    }
}



tile_type_t get_tile_type(int y, int x)
{
    if (on_map(y, x))
        return level.map[y][x].type;

    return tile_void;
}



