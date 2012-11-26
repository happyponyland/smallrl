#include <stdlib.h>

#include "level.h"

static void clear_level(level_t *);
static void set_tile_flags_by_type(tile_t *, tile_type_t);

int on_map(level_t * level, int y, int x)
{
	if (y >= 0 && x >= 0 && y < MAP_H && x < MAP_W)
		return 1;

	return 0;
}

level_t * create_new_level(level_t * old_level)
{
	level_t * new_level = malloc(sizeof(level_t));

	clear_level(new_level);

	new_level->mobs[0].type = mob_player;
	new_level->up_level = old_level;
	new_level->depth = old_level != NULL ? old_level->depth + 1 : 0;

	return new_level;
}

static void clear_level(level_t * level)
{
	int y;
	int x;
	int i;

	for (i = 1; i < MAX_MOBS_PER_LEVEL; i++)
		level->mobs[i].type = mob_none;

	for (y = 0; y < MAP_H; y++)
		for (x = 0; x < MAP_W; x++) {
			set_tile(level, y, x, tile_void);
			level->map[y][x].is_explored = 0;
			level->map[y][x].is_lit = 0;
		}

	return;
}


void set_tile(level_t * level, int y, int x, tile_type_t t)
{
	if (on_map(level, y, x)) {
		level->map[y][x].type = t;
		set_tile_flags_by_type(&level->map[y][x], t);
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

tile_type_t get_tile_type(level_t * level, int y, int x)
{
	if (on_map(level, y, x))
		return level->map[y][x].type;

	return tile_void;
}

int try_move_mob(level_t * level, mob_t * mob_to_move, int y_speed, int x_speed)
{
	int new_y = mob_to_move->y + y_speed;
	int new_x = mob_to_move->x + x_speed;

	if (on_map(level, new_y, new_x) &&
		!(level->map[new_y][new_x].flags & tile_unpassable))
	{
		mob_to_move->y = new_y;
		mob_to_move->x = new_x;

		return 1;
	}

	return 0;
}

int try_make_mob(level_t * level, mob_type_t type, int y, int x)
{
	int i;

	for (i = 0; i < MAX_MOBS_PER_LEVEL; i++)
	{
		if (level->mobs[i].type == mob_none)
		{
			level->mobs[i].y = y;
			level->mobs[i].x = x;

			level->mobs[i].type = type;

			switch (type)
			{
			case mob_newbie:
				level->mobs[i].hp = 5;
				break;

			default:
				level->mobs[i].hp = 2;
				break;
			}

			return 1;
		}
	}

	return 0;
}



int get_mob(level_t * level, int y, int x)
{
	int i;

	for (i = 0; i < MAX_MOBS_PER_LEVEL; i++)
		if (level->mobs[i].type && level->mobs[i].y == y && level->mobs[i].x == x)
			return i;

	return -1;
}
