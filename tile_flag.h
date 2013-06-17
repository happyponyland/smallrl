#ifndef SMALLRL_TYPE_TILE_FLAG_T
#define SMALLRL_TYPE_TILE_FLAG_T

typedef enum tile_flag_e
{
    tile_none = 0x0,
    tile_unpassable = 0x1,
    tile_permalit = 0x2,
    tile_noflood = 0x4
} tile_flag_t;

#endif
