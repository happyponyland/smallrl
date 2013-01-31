#ifndef GEN_STANDARD_DUNGEON_INTERNAL_H
#define GEN_STANDARD_DUNGEON_INTERNAL_H

#include <stdbool.h>
#include "level.h"
#include "item.h"

typedef struct point_s
{
    int x;
    int y;
} point_t;

typedef struct range_segment_s
{
    int initial;
    int length;
} range_segment_t;

typedef struct rectangle_s
{
    int top;
    int bottom;
    int left;
    int right;
} rectangle_t;

typedef struct split_region_s
{
    rectangle_t first;
    rectangle_t second;
} split_region_t;

typedef struct bsp_node_s
{
    int level;

    rectangle_t * region;
    rectangle_t * room_region;

    struct bsp_node_s * right;
    struct bsp_node_s * left;
} bsp_node_t;

typedef struct bsp_head_s
{
    level_t * level;
    bsp_node_t * root_node;
} bsp_head_t;

#endif
