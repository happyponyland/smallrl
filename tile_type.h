#ifndef SMALLRL_TYPE_TILE_TYPE_T
#define SMALLRL_TYPE_TILE_TYPE_T

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
    tile_stair
} tile_type_t;

#endif
