#ifndef SMALLRL_LEVEL_H
#define SMALLRL_LEVEL_H

#include "tile.h"
#include "mob.h"

#define MAX_MOBS_PER_LEVEL 40

typedef struct level_s
{
    int width;
    int height;

    tile_t * map;
    mob_t mobs[MAX_MOBS_PER_LEVEL];
    int depth;
    struct level_s * down_level;
    struct level_s * up_level;
} level_t;

/* List of all levels */
extern level_t * levels;

/* Pointer to current level */
extern level_t * current_level;

/*--------*/

/* Prototypes */
level_t * create_new_level(level_t *, int, int);
int on_map(level_t *, int y, int x);
tile_type_t get_tile_type(level_t *, int, int);
void set_tile(level_t *, int, int, tile_type_t);

int try_move_mob(level_t *, mob_t *, int, int);
int try_make_mob(level_t *, mob_type_t, int, int);
int get_mob(level_t *, int, int);

#endif
