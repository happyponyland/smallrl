#ifndef SMALLRL_TILE_H
#define SMALLRL_TILE_H

#include <stdint.h>
#include <stdbool.h>

#include "item.h"

#include "tile_type.h"
#include "tile_flag.h"

typedef struct tile_s tile_t;

struct tile_s
{
    tile_type_t type;
    tile_flag_t flags;
    bool is_explored;
    bool is_lit;
    bool is_periphery;
    bool is_path;
    item_t * item;
};

#endif
