#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "standard_dungeon.h"
#include "standard_dungeon_internal.h"

static bool is_too_large(rectangle_t *);
static bsp_node_t * create_new_node(int, int, int, int, int);
static split_region_t * create_split_region(int, rectangle_t *);
static rectangle_t * create_room_region(rectangle_t *);
static void paint_rooms(bsp_head_t *);
static void connect_all_rooms(bsp_head_t *);
static void connect_room(level_t *, bsp_node_t *, bsp_node_t *, bool);
static void paint(level_t *, rectangle_t *, tile_type_t);
static void paint_outline(level_t *, rectangle_t *);
static void paint_corridor(level_t *, point_t, point_t, tile_type_t);
static bsp_node_t * find_best_node(bsp_node_t *, bool, bool);
static range_segment_t get_best_horizontal_overlap(rectangle_t *, rectangle_t *);
static range_segment_t get_best_vertical_overlap(rectangle_t *, rectangle_t *);
static int max(int, int);
static int min(int, int);
static void release_nodes(bsp_node_t *);
static void place_monsters(bsp_head_t *);
static mob_type_t random_mob_type(int);
static void place_items(bsp_head_t *);
static void place_stairs(bsp_head_t *);
static void fix_tee_walls(level_t *);

void create_standard_dungeon(level_t * level, point_t * player_start_position)
{
    bsp_node_t * stack[1000];
    int stack_pointer;
    bsp_head_t bsp_head;

    stack_pointer = 0;
    bsp_head.level = level;
    bsp_head.root_node = create_new_node(0, 0, level->height - 1, 0, level->width - 1);

    stack[stack_pointer++] = bsp_head.root_node;

    while(stack_pointer > 0)
    {
        bsp_node_t * current_node = stack[--stack_pointer];

        if(is_too_large(current_node->region))
        {
            split_region_t * split_region = create_split_region(current_node->level, current_node->region);

            free(current_node->region);
            current_node->region = NULL;

            current_node->left = create_new_node(
                current_node->level + 1,
                split_region->first.top,
                split_region->first.bottom,
                split_region->first.left,
                split_region->first.right);

            current_node->right = create_new_node(
                current_node->level + 1,
                split_region->second.top,
                split_region->second.bottom,
                split_region->second.left,
                split_region->second.right);

            free(split_region);

            stack[stack_pointer++] = current_node->right;
            stack[stack_pointer++] = current_node->left;
        }
    }

    paint_rooms(&bsp_head);

    // Rooms that overlap need to have their walls fixed in tee-spots
    fix_tee_walls(level);

    connect_all_rooms(&bsp_head);

    bsp_node_t * start_room = find_best_node(bsp_head.root_node, true, true);

    (*player_start_position).x = start_room->room_region->left + 2;
    (*player_start_position).y = start_room->room_region->top + 2;

    place_monsters(&bsp_head);

    place_items(&bsp_head);

    place_stairs(&bsp_head);

    //REVIEW: Might be a good idea to keep the room tree available for map modification spells and teleportation purposes?
    release_nodes(bsp_head.root_node);
}

static void fix_tee_walls(level_t * level)
{
    for(int y = 1; y < level->height - 1; y += 1)
    {
        for(int x = 1; x < level->width - 1; x += 1)
        {
            tile_type_t above = get_tile_type(level, y - 1, x);
            tile_type_t below = get_tile_type(level, y + 1, x);
            tile_type_t left = get_tile_type(level, y, x - 1);
            tile_type_t right = get_tile_type(level, y, x + 1);

            if((above == tile_wall_l || above == tile_wall_r || above == tile_wall_ur || above == tile_wall_ul)
               && (below == tile_wall_l || below == tile_wall_r || below == tile_wall_lr || below == tile_wall_ll)
               && (left == tile_wall_t || left == tile_wall_b || left == tile_wall_ul || left == tile_wall_ll)
               && (right == tile_wall_t || right == tile_wall_b || right == tile_wall_ur || right == tile_wall_lr))
            {
                set_tile(level, y, x, tile_wall_plus);
            }
            else if((above == tile_wall_l || above == tile_wall_r || above == tile_wall_ur || above == tile_wall_ul)
                    && (left == tile_wall_t || left == tile_wall_b || left == tile_wall_ul || left == tile_wall_ll)
                    && (right == tile_wall_t || right == tile_wall_b || right == tile_wall_ur || right == tile_wall_lr))
            {
                set_tile(level, y, x, tile_wall_btee);
            }
            else if((below == tile_wall_l || below == tile_wall_r || below == tile_wall_lr || below == tile_wall_ll)
                    && (left == tile_wall_t || left == tile_wall_b || left == tile_wall_ul || left == tile_wall_ll)
                    && (right == tile_wall_t || right == tile_wall_b || right == tile_wall_ur || right == tile_wall_lr))
            {
                set_tile(level, y, x, tile_wall_ttee);
            }
            else if((above == tile_wall_l || above == tile_wall_r || above == tile_wall_ur || above == tile_wall_ul)
                    && (below == tile_wall_l || below == tile_wall_r || below == tile_wall_lr || below == tile_wall_ll)
                    && (left == tile_wall_t || left == tile_wall_b || left == tile_wall_ul || left == tile_wall_ll))
            {
                set_tile(level, y, x, tile_wall_rtee);
            }
            else if((above == tile_wall_l || above == tile_wall_r || above == tile_wall_ur || above == tile_wall_ul)
                    && (below == tile_wall_l || below == tile_wall_r || below == tile_wall_lr || below == tile_wall_ll)
                    && (right == tile_wall_t || right == tile_wall_b || right == tile_wall_ur || right == tile_wall_lr))
            {
                set_tile(level, y, x, tile_wall_ltee);
            }
        }
    }
}


static void place_monsters(bsp_head_t * head)
{
    bsp_node_t * walk_stack[1000];
    int walk_stack_pointer;

    walk_stack_pointer = 0;

    walk_stack[walk_stack_pointer++] = head->root_node;

    while(walk_stack_pointer > 0)
    {
        bsp_node_t * current_node = walk_stack[--walk_stack_pointer];

        if(current_node->room_region != NULL)
        {
            int x = (current_node->room_region->left + 1) + (rand() % (current_node->room_region->right - current_node->room_region->left - 2));
            int y = (current_node->room_region->top + 1) + (rand() % (current_node->room_region->bottom - current_node->room_region->top - 2));

            try_make_mob(head->level,
                         random_mob_type(head->level->depth),
                         y, x);
        }

        if(current_node->right != NULL)
            walk_stack[walk_stack_pointer++] = current_node->right;

        if(current_node->left != NULL)
            walk_stack[walk_stack_pointer++] = current_node->left;
    }
}

static mob_type_t random_mob_type(int depth)
{
    switch((depth / 2) + rand() % (depth + 2))
    {
    case 0:
        return mob_newbie;
    case 1:
        return mob_zombie;
    case 2:
        return mob_1337hax0r;
    case 3:
        return mob_hobbyist;
    case 4:
        return mob_war_troll;
    default:
        return mob_zombie;
    }
}

static void place_items(bsp_head_t * head)
{
    bsp_node_t * walk_stack[1000];
    int walk_stack_pointer;

    walk_stack_pointer = 0;

    walk_stack[walk_stack_pointer++] = head->root_node;

    while(walk_stack_pointer > 0)
    {
        bsp_node_t * current_node = walk_stack[--walk_stack_pointer];

        if(current_node->room_region != NULL)
        {
            int x = (current_node->room_region->left + 1) + (rand() % (current_node->room_region->right - current_node->room_region->left - 2));
            int y = (current_node->room_region->top + 1) + (rand() % (current_node->room_region->bottom - current_node->room_region->top - 2));

            if (head->level->map[y * head->level->width + x].item == 0)
            {
                head->level->map[y * head->level->width + x].item = random_item();
            }
        }

        if(current_node->right != NULL)
            walk_stack[walk_stack_pointer++] = current_node->right;

        if(current_node->left != NULL)
            walk_stack[walk_stack_pointer++] = current_node->left;
    }
}

static void place_stairs(bsp_head_t * head)
{
    bsp_node_t * stair_node = find_best_node(head->root_node, false, false);

    int x = (stair_node->room_region->left + 1) + (rand() % (stair_node->room_region->right - stair_node->room_region->left - 2));
    int y = (stair_node->room_region->top + 1) + (rand() % (stair_node->room_region->bottom - stair_node->room_region->top - 2));

    set_tile(head->level, y, x, tile_stair);
}

static void release_nodes(bsp_node_t * node)
{
    bsp_node_t * walk_stack[1000];
    int walk_stack_pointer;

    walk_stack_pointer = 0;
    walk_stack[walk_stack_pointer++] = node;


    while(walk_stack_pointer > 0)
    {
        bsp_node_t * current_node = walk_stack[--walk_stack_pointer];

        if(current_node->right != NULL)
        {
            walk_stack[walk_stack_pointer++] = current_node->right;
        }

        if(current_node->left != NULL)
        {
            walk_stack[walk_stack_pointer++] = current_node->left;
        }

        // Release regions
        if(current_node->region != NULL) {
            free(current_node->region);
            current_node->region = NULL;
        }

        if(current_node->room_region != NULL) {
            free(current_node->room_region);
            current_node->room_region = NULL;
        }

        // Release node.
        free(current_node);
        current_node = NULL;
    }
}

static void connect_all_rooms(bsp_head_t * head)
{
    bsp_node_t * walk_stack[1000];
    int walk_stack_pointer;

    bool left_stack[1000];
    int left_stack_pointer;

    bool top_stack[1000];
    int top_stack_pointer;

    level_t * level;

    level = head->level;

    left_stack_pointer = 0;

    top_stack_pointer = 0;
    top_stack[top_stack_pointer++] = false;

    walk_stack_pointer = 0;
    walk_stack[walk_stack_pointer++] = head->root_node;

    while(walk_stack_pointer > 0)
    {
        bsp_node_t * current_node = walk_stack[--walk_stack_pointer];

        bool is_leaf = current_node->left == NULL && current_node->right == NULL;

        if(is_leaf)
            continue;

        bool is_split_vertically = current_node->level % 2 == 0;

        bsp_node_t * left;
        bsp_node_t * right;

        if(is_split_vertically) {
            bool is_top = top_stack[--top_stack_pointer];
            left = find_best_node(current_node->left, is_top, false);
            if(left == NULL)
                left = current_node;

            right = find_best_node(current_node->right, is_top, true);
            if(right == NULL)
                right = current_node;
        } else {
            bool is_left = left_stack[--left_stack_pointer];
            left = find_best_node(current_node->left, false, is_left);
            if(left == NULL)
                left = current_node;

            right = find_best_node(current_node->right, true, is_left);
            if(right == NULL)
                right = current_node;
        }

        if(left != right)
            connect_room(level, left, right, is_split_vertically);

        if(current_node->right != NULL)
        {

            if(is_split_vertically) {
                left_stack[left_stack_pointer++] = false;
            } else {
                top_stack[top_stack_pointer++] = false;
            }

            walk_stack[walk_stack_pointer++] = current_node->right;
        }

        if(current_node->left != NULL)
        {
            if(is_split_vertically) {
                left_stack[left_stack_pointer++] = true;
            } else {
                top_stack[top_stack_pointer++] = true;
            }

            walk_stack[walk_stack_pointer++] = current_node->left;
        }
    }
}

static bsp_node_t * find_best_node(bsp_node_t * node, bool want_top, bool want_left)
{
    bsp_node_t * walk_stack[1000];
    int walk_stack_pointer;

    walk_stack_pointer = 0;
    walk_stack[walk_stack_pointer++] = node;

    while(walk_stack_pointer > 0)
    {
        bsp_node_t * current_node = walk_stack[--walk_stack_pointer];

        if(current_node == NULL)
            return NULL;

        bool is_split_vertically = current_node->level % 2 == 0;
        bool is_leaf = current_node->left == NULL && current_node->right == NULL;

        if(is_leaf)
            return current_node;

        if((is_split_vertically && want_left) || (!is_split_vertically && want_top)) {
            if(current_node->right != NULL)
                walk_stack[walk_stack_pointer++] = current_node->right;

            if(current_node->left != NULL)
                walk_stack[walk_stack_pointer++] = current_node->left;
        }

        if((is_split_vertically && !want_left) || (!is_split_vertically && !want_top)) {
            if(current_node->left != NULL)
                walk_stack[walk_stack_pointer++] = current_node->left;

            if(current_node->right != NULL)
                walk_stack[walk_stack_pointer++] = current_node->right;
        }
    }

    return NULL;
}

static void connect_room(level_t * level, bsp_node_t * first, bsp_node_t * second, bool is_split_vertically)
{
    point_t first_point;
    point_t second_point;

    if(is_split_vertically) {
        range_segment_t first_range = get_best_vertical_overlap(first->room_region, second->room_region);
        range_segment_t second_range = get_best_vertical_overlap(second->room_region, first->room_region);

        first_point = (point_t) { .x = first->room_region->left + 1, .y = first_range.initial };

        second_point = (point_t) { .x = second->room_region->left + 1, . y = second_range.initial };

        if(first_range.length > 0 && second_range.length > 0) {
            int extra_y = rand() % first_range.length;
            first_point.y += extra_y;
            second_point.y += extra_y;
        }

        if(first_point.y == second_point.y)
        {
            paint_corridor(level, first_point, second_point, tile_corridor);
        }
        else
        {
            point_t top_point = first_point.y < second_point.y ? first_point : second_point;
            point_t bottom_point = first_point.y < second_point.y ? second_point : first_point;

            point_t mid_point_1 = (point_t) { .x = top_point.x, .y = top_point.y + (abs(bottom_point.y - top_point.y) / 2) };
            point_t mid_point_2 = (point_t) { .x = bottom_point.x, .y = top_point.y + (abs(bottom_point.y - top_point.y) / 2) };
            paint_corridor(level, top_point, mid_point_1, tile_corridor);
            paint_corridor(level, mid_point_1, mid_point_2, tile_corridor);
            paint_corridor(level, mid_point_2, bottom_point, tile_corridor);
        }

    } else {
        range_segment_t first_range = get_best_horizontal_overlap(first->room_region, second->room_region);
        range_segment_t second_range = get_best_horizontal_overlap(second->room_region, first->room_region);

        first_point = (point_t) { .x = first_range.initial, .y = first->room_region->top + 1 };

        second_point = (point_t) { .x = second_range.initial, . y = second->room_region->top + 1 };

        if(first_range.length > 0 && second_range.length > 0) {
            int extra_x = rand() % first_range.length;
            first_point.x += extra_x;
            second_point.x += extra_x;
        }

        if(first_point.x == second_point.x)
        {
            paint_corridor(level, first_point, second_point, tile_corridor);
        }
        else
        {
            point_t left_point = first_point.x < second_point.x ? first_point : second_point;
            point_t right_point = first_point.x < second_point.x ? second_point : first_point;

            point_t mid_point_1 = (point_t) { .x = left_point.x + (abs(right_point.x - left_point.x) / 2), .y = left_point.y };
            point_t mid_point_2 = (point_t) { .x = left_point.x + (abs(right_point.x - left_point.x) / 2), .y = right_point.y };
            paint_corridor(level, left_point, mid_point_1, tile_corridor);
            paint_corridor(level, mid_point_1, mid_point_2, tile_corridor);
            paint_corridor(level, mid_point_2, right_point, tile_corridor);
        }
    }
}

static range_segment_t get_best_horizontal_overlap(rectangle_t * first, rectangle_t * second)
{
    if(first->left > second->right)
    {
        return (range_segment_t) { .initial = first->left + 1, .length = 0 };
    }

    if(first->right < second->left)
    {
        return (range_segment_t) { .initial = first->right - 1, .length = 0 };
    }

    int start = max(first->left + 1, second->left + 1);
    int length = abs(start - min(first->right - 1, second->right - 1));

    return (range_segment_t) { .initial = start, .length = length };
}

static range_segment_t get_best_vertical_overlap(rectangle_t * first, rectangle_t * second)
{
    if(first->top > second->bottom)
    {
        return (range_segment_t) { .initial = first->top + 1, .length = 0 };
    }

    if(first->bottom < second->top)
    {
        return (range_segment_t) { .initial = first->bottom - 1, .length = 0 };
    }

    int start = max(first->top + 1, second->top + 1);
    int length = abs(start - min(first->bottom - 1, second->bottom - 1));
    return (range_segment_t) { .initial = start, .length = length };
}

static inline int max(int first, int second)
{
    return first > second ? first : second;
}

static inline int min(int first, int second)
{
    return first < second ? first : second;
}

static void paint_corridor(level_t * level, point_t start, point_t end, tile_type_t tile_type)
{
    int delta_x = abs(end.x - start.x);
    int delta_y = abs(end.y - start.y);

    int step_x = start.x < end.x ? 1 : -1;
    int step_y = start.y < end.y ? 1 : -1;

    int err = delta_x - delta_y;

    int x = start.x;
    int y = start.y;

    int end_x = end.x;
    int end_y = end.y;

    for(;;)
    {
        tile_type_t current_tile_type = level->map[y * level->width + x].type;
        tile_type_t new_tile_type = current_tile_type;

        if(current_tile_type == tile_void)
            new_tile_type = tile_type;
        else if(current_tile_type == tile_wall_t ||
                current_tile_type == tile_wall_b ||
                current_tile_type == tile_wall_r ||
                current_tile_type == tile_wall_l ||
                current_tile_type == tile_wall_ll ||
                current_tile_type == tile_wall_lr ||
                current_tile_type == tile_wall_ul ||
                current_tile_type == tile_wall_ur)
            new_tile_type = tile_doorway;

        set_tile(level, y, x, new_tile_type);

        if (x == end_x && y == end_y)
            break;

        int err2 = 2 * err;

        if(err2 > -delta_y)
        {
            err -= delta_y;
            x += step_x;
        }

        if(err2 < delta_x)
        {
           err += delta_x;
           y += step_y;
        }
    }
}

static void paint_rooms(bsp_head_t * head)
{
    bsp_node_t * stack[1000];
    int stack_pointer;
    level_t * level;

    level = head->level;

    stack_pointer = 0;

    stack[stack_pointer++] = head->root_node;

    while(stack_pointer > 0)
    {
        bsp_node_t * current_node = stack[--stack_pointer];

        if(current_node->region != NULL) {
            rectangle_t * region = current_node->region;

            current_node->room_region = create_room_region(region);

            paint(level, current_node->room_region, tile_floor);

            paint_outline(level, current_node->room_region);
        }

        if(current_node->left != NULL)
            stack[stack_pointer++] = current_node->left;

        if(current_node->right != NULL)
            stack[stack_pointer++] = current_node->right;
    }
}

static rectangle_t * create_room_region(rectangle_t * region)
{
    int half_width = (rand() % (region->right - region->left - 2)) / 2;
    int half_height = (rand() % (region->bottom - region->top - 2)) / 2;

    rectangle_t * room_region = malloc(sizeof(rectangle_t));
    room_region->top = region->top + half_height;
    room_region->bottom = region->bottom - half_height;
    room_region->left = region->left + half_width;
    room_region->right = region->right - half_width;

    return room_region;
}

static void paint(level_t * level, rectangle_t * region, tile_type_t tile_type)
{
    for(int y = region->top; y < region->bottom; y += 1)
    {
        for(int x = region->left; x < region->right; x += 1)
        {
            set_tile(level, y, x, tile_type);
        }
    }
}

static void paint_outline(level_t * level, rectangle_t * region)
{
    for(int y = region->top; y < region->bottom; y += 1)
    {
        set_tile(level, y, region->left, tile_wall_l);
        set_tile(level, y, region->right, tile_wall_r);
    }

    for(int x = region->left; x < region->right; x += 1)
    {
        set_tile(level, region->top, x, tile_wall_t);
        set_tile(level, region->bottom, x, tile_wall_b);
    }

    set_tile(level, region->top, region->left, tile_wall_ul);
    set_tile(level, region->top, region->right, tile_wall_ur);
    set_tile(level, region->bottom, region->left, tile_wall_ll);
    set_tile(level, region->bottom, region->right, tile_wall_lr);
}

static split_region_t * create_split_region(int level, rectangle_t * region)
{
    rectangle_t first;
    rectangle_t second;

    if(level % 2 == 0) {
        int mid_x = (region->right - region->left) / 2;
        int random_max_x = rand() % (mid_x / 2);

        int split_x = random_max_x == 0 ? mid_x : -random_max_x + mid_x + (rand() % random_max_x);

        first = (rectangle_t) {
            .left = region->left,
            .right = region->left + split_x,
            .top = region->top,
            .bottom = region->bottom
        };

        second = (rectangle_t) {
            .left = region->left + split_x,
            .right = region->right,
            .top = region->top,
            .bottom = region->bottom
        };
    } else {
        int mid_y = (region->bottom - region->top) / 2;
        int random_max_y = rand() % (mid_y / 2);

        int split_y = random_max_y == 0 ? mid_y : -random_max_y + mid_y + (rand() % random_max_y);

        first = (rectangle_t) {
            .left = region->left,
            .right = region->right,
            .top = region->top,
            .bottom = region->top + split_y
        };

        second = (rectangle_t) {
            .left = region->left,
            .right = region->right,
            .top = region->top + split_y,
            .bottom = region->bottom
        };
    }

    split_region_t * split_region = malloc(sizeof(split_region_t));

    split_region->first = (rectangle_t) {
        .left = first.left,
        .right = first.right,
        .top = first.top,
        .bottom = first.bottom
    };

    split_region->second = (rectangle_t) {
        .left = second.left,
        .right = second.right,
        .top = second.top,
        .bottom = second.bottom
    };

    return split_region;
}

static bsp_node_t * create_new_node(int level, int top, int bottom, int left, int right)
{
    bsp_node_t * new_node = malloc(sizeof(bsp_node_t));

    new_node->level = level;
    new_node->left = NULL;
    new_node->right = NULL;

    rectangle_t * new_rectangle = malloc(sizeof(rectangle_t));

    new_rectangle->top = top;
    new_rectangle->bottom = bottom;
    new_rectangle->left = left;
    new_rectangle->right = right;

    new_node->region = new_rectangle;
    new_node->room_region = NULL;

    return new_node;
}

static bool is_too_large(rectangle_t * region)
{
    if(region == NULL)
        return false;

    int height = region->bottom - region->top;
    int width = region->right - region->left;

    if(height < 10 && width < 10)
        return false;

    if(height < 10)
        return false;

    if(width < 10)
        return false;

    if(height * width < 100)
        return false;

    return true;
}
