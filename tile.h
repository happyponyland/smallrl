#ifndef SMALLRL_TILE_H
#define SMALLRL_TILE_H

typedef enum tile_type_e
{
	tile_void,
	tile_floor,
	tile_wall_t,
	tile_wall_b,
	tile_wall_r,
	tile_wall_l,
	tile_wall_ll,
	tile_wall_lr,
	tile_wall_ul,
	tile_wall_ur,
	tile_corridor,
	tile_stair
} tile_type_t;

typedef enum tile_flag_e
{
	tile_none = 0x0,
	tile_unpassable = 0x1,
	tile_permalit = 0x2
} tile_flag_t;

typedef struct tile_s
{
	tile_type_t type;
	tile_flag_t flags;
	int is_explored;
	int is_lit;
	int is_path;
} tile_t;

#endif
