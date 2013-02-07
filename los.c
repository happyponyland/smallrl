#include <stdlib.h>

#include "los.h"

static void los(level_t *, point_t, int);
static void trace(level_t *, point_t, point_t);

void explore_map(level_t * level, point_t center_position)
{
    int top = current_level->view.ul_position.y;
    int bottom = current_level->view.ul_position.y + current_level->view.height;

    int left = current_level->view.ul_position.x;
    int right = current_level->view.ul_position.x + current_level->view.width;

    for (int y = top; y < bottom; y += 1)
        for (int x = left; x < right; x += 1)
            level->map[y * current_level->width + x].is_lit = false;

    los(level, center_position, 10);
}

static void los(level_t * level, point_t center_position, int radius)
{
    int cx = center_position.x;
    int cy = center_position.y;

    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    trace(level, center_position, (point_t) { .x = cx, .y = cy + radius });
    trace(level, center_position, (point_t) { .x = cx, .y = cy - radius });
    trace(level, center_position, (point_t) { .x = cx + radius, .y = cy });
    trace(level, center_position, (point_t) { .x = cx - radius, .y = cy });

    while(x < y)
    {
        if(f >= 0)
        {
            y -= 1;
            ddF_y += 2;
            f += ddF_y;
        }

        x += 1;

        ddF_x += 2;
        f += ddF_x;

        trace(level, center_position, (point_t) { .x = cx + x, .y = cy + y });
        trace(level, center_position, (point_t) { .x = cx - x, .y = cy + y });
        trace(level, center_position, (point_t) { .x = cx + x, .y = cy - y });
        trace(level, center_position, (point_t) { .x = cx - x, .y = cy - y });
        trace(level, center_position, (point_t) { .x = cx + y, .y = cy + x });
        trace(level, center_position, (point_t) { .x = cx - y, .y = cy + x });
        trace(level, center_position, (point_t) { .x = cx + y, .y = cy - x });
        trace(level, center_position, (point_t) { .x = cx - y, .y = cy - x });
  }
}

static void trace(level_t * level, point_t start, point_t end)
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
        if(on_map(level, y, x)) {
            level->map[y * current_level->width + x].is_explored = true;
            level->map[y * current_level->width + x].is_lit = true;

            if((level->map[y * current_level->width + x].flags & tile_unpassable) == tile_unpassable)
                break;
        }

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
