#ifndef SMALLRL_TILE_H
#define SMALLRL_TILE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum tile_type_e
{
    tile_void,
    tile_floor,
    tile_doorway,
    tile_wall_t,
    tile_wall_b,
    tile_wall_r,
    tile_wall_l,
    tile_wall_ll,
    tile_wall_lr,
    tile_wall_ul,
    tile_wall_ur,
    tile_wall_ttee,
    tile_wall_btee,
    tile_wall_rtee,
    tile_wall_ltee,
    tile_wall_plus,
    tile_corridor,
    tile_door_h,
    tile_door_v,
    tile_stair
} tile_type_t;

typedef enum tile_flag_e
{
    tile_none = 0x0,
    tile_unpassable = 0x1,
    tile_permalit = 0x2,
    tile_noflood = 0x4
} tile_flag_t;

typedef struct tile_s
{
    tile_type_t type;
    tile_flag_t flags;
    bool is_explored;
    bool is_lit;
    bool is_path;
    uint32_t item;
} tile_t;

#endif
