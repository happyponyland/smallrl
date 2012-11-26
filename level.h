#ifndef SMALLRL_LEVEL_H
#define SMALLRL_LEVEL_H

#include "tile.h"
#include "mob.h"

#define MAP_H 20
#define MAP_W 80

#define MAX_MOBS_PER_LEVEL 40

typedef struct level_s
{
    tile_t map[MAP_H][MAP_W];
    mob_t mobs[MAX_MOBS_PER_LEVEL];
    int depth;
    struct level_s * down_level;
    struct level_s * up_level;
} level_t;

/* List of all levels */
level_t * levels;

/* Pointer to current level */
level_t * current_level;

/*--------*/

/* Prototypes */
level_t * create_new_level(level_t *);
int on_map(level_t *, int y, int x);
tile_type_t get_tile_type(level_t *, int, int);
void set_tile(level_t *, int, int, tile_type_t);

int try_move_mob(level_t *, mob_t *, int, int);
int try_make_mob(level_t *, mob_type_t, int, int);
int get_mob(level_t *, int, int);

#endif
