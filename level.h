#ifndef SMALLRL_LEVEL_H
#define SMALLRL_LEVEL_H

#include <stdbool.h>

#include "types.h"
#include "tile.h"
#include "mob.h"

#define MAX_MOBS_PER_LEVEL 40

typedef struct window_s window_t;
struct window_s
{
    point_t ul_position;

    int width;
    int height;
};

typedef struct level_s level_t;
struct level_s
{
    int width;
    int height;

    tile_t * map;
    window_t view;

    mob_t mobs[MAX_MOBS_PER_LEVEL];
    int depth;
    level_t * down_level;
    level_t * up_level;
};

level_t * create_new_level(level_t *, int, int);
bool on_map(level_t *, int y, int x);
tile_type_t get_tile_type(level_t *, int, int);
void set_tile(level_t *, int, int, tile_type_t);

bool try_make_mob(level_t *, mob_type_t, int, int);
int get_mob(level_t *, int, int);
void center_view(level_t *, point_t);

#endif
