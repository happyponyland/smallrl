#include "smallrl.h"


int on_map(int y, int x)
{
    if (y >= 0 && x >= 0 && y < MAP_H && x < MAP_W)
        return 1;

    return 0;
}


void clear_map()
{
    int y;
    int x;
    int i;

    for (i = 1; i < MOBS; i++)
        mob[i].type = mob_none;

    for (y = 0; y < MAP_H; y++)
        for (x = 0; x < MAP_W; x++)
            level.map[y][x] = tile_floor;

    return;
}


void boring_level(int* startx, int* starty)
{
    int rooms;

    int room_t[9];
    int room_l[9];
    int room_b[9];
    int room_r[9];

    int y;
    int x;
    int i;

    /* Clear the map */
    for (y = 0; y < MAP_H; y++)
        for (x = 0; x < MAP_W; x++)
            level.map[y][x] = tile_void;

    /* Place random rooms */
    rooms = 0;

    for (y = 0; y < 2; y++)
    {
        for (x = 0; x < 3; x++)
        {
            room_t[rooms] = y * ((float)MAP_H / 2) + 1;
            room_l[rooms] = x * ((float)MAP_W / 3) + 1;
            room_b[rooms] = (y + 1) * ((float)MAP_H / 2) - 2;
            room_r[rooms] = (x + 1) * ((float)MAP_W / 3) - 2;

            rooms++;
        }
    }

    for (i = 0; i < rooms; i++)
    {
        room_t[i] += 1 + rand() % 3;
        room_b[i] -= 1 + rand() % 3;

        room_l[i] += 1 + rand() % 11;
        room_r[i] -= 1 + rand() % 11;
    }

    for (i = 0; i < rooms; i++)
        build_room(room_t[i], room_l[i], room_b[i], room_r[i], 0);

    int a;
    int b;
    int c;

    c = 4;

    for (i = 0; i < rooms; i++)
    {
        a = i;
        b = (b + c) % rooms;

        connect_rooms(room_t[a] + rand() % (room_b[a] - room_t[a]),
                      room_l[a] + rand() % (room_r[a] - room_l[a]),
                      room_t[b] + rand() % (room_b[b] - room_t[b]),
                      room_l[b] + rand() % (room_r[b] - room_l[b]));
    }

    for (i = 0; i < rooms; i++)
    {
        build_room(room_t[i], room_l[i], room_b[i], room_r[i], 0);
    }

    int random_room;
    random_room = rand() % rooms;

    *starty = room_t[random_room] + rand() % (room_b[random_room] - room_t[random_room]);
    *startx = room_l[random_room] + rand() % (room_r[random_room] - room_l[random_room]);

    return;
}



void build_room(int t, int l, int b, int r, int flags)
{
    int y;
    int x;

    for (y = t; y <= b; y++)
    {
        if (get_tile(y, l - 1) == tile_corridor)
            set_tile(y, l - 1, tile_floor);
        else
            set_tile(y, l - 1, tile_wall_l);

        if (get_tile(y, r + 1) == tile_corridor)
            set_tile(y, r + 1, tile_floor);
        else
            set_tile(y, r + 1, tile_wall_r);
    }

    for (x = l; x <= r; x++)
    {
        if (get_tile(t - 1, x) == tile_corridor)
            set_tile(t - 1, x, tile_floor);
        else
            set_tile(t - 1, x, tile_wall_t);

        if (get_tile(b + 1, x) == tile_corridor)
            set_tile(b + 1, x, tile_floor);
        else
            set_tile(b + 1, x, tile_wall_b);
    }

    set_tile(t - 1, l - 1, tile_wall_ul);
    set_tile(b + 1, l - 1, tile_wall_ll);
    set_tile(b + 1, r + 1, tile_wall_lr);
    set_tile(t - 1, r + 1, tile_wall_ur);

    for (y = t; y <= b; y++)
        for (x = l; x <= r; x++)
            level.map[y][x] = tile_floor;

    return;
}



void set_tile(int y, int x, tile_t t)
{
    if (on_map(y, x))
        level.map[y][x] = t;

    return;
}


tile_t get_tile(int y, int x)
{
    if (on_map(y, x))
        return level.map[y][x];

    return tile_void;
}



void connect_rooms(int y1, int x1, int y2, int x2)
{
    if (rand() % 2 == 0)
    {
        h_corridor(y1, x1, x2);
        v_corridor(y1, y2, x2);
    }
    else
    {
        h_corridor(y1, x1, x2);
        v_corridor(y1, y2, x2);
    }

    return;
}


void h_corridor(int y, int x1, int x2)
{
    int x;
    tile_t t;

    if (x2 < x1)
    {
        x = x1;
        x1 = x2;
        x2 = x;
    }

    for (x = x1; x <= x2; x++)
    {
        if (get_tile(y, x - 1) == tile_wall_lr ||
            //get_tile(y, x - 1) == tile_wall_ll ||
            get_tile(y, x + 1) == tile_wall_ll //||
            /*get_tile(y, x + 1) == tile_wall_lr*/)
        {
            set_tile(y, x, tile_corridor);
            set_tile(y + 1, x, tile_corridor);
        }

        if (get_tile(y, x - 1) == tile_wall_ur ||
            //get_tile(y, x - 1) == tile_wall_ul ||
            get_tile(y, x + 1) == tile_wall_ul //||
            /*get_tile(y, x + 1) == tile_wall_ur*/)
        {
            set_tile(y, x, tile_corridor);
            set_tile(y - 1, x, tile_corridor);
        }

        t = get_tile(y, x);

        if (t == tile_wall_lr ||
            t == tile_wall_ll ||
            t == tile_wall_b)
        {
            set_tile(y + 1, x, tile_corridor);
        }
        else if (t == tile_wall_ur ||
                 t == tile_wall_ul ||
                 t == tile_wall_t)
        {
            set_tile(y - 1, x, tile_corridor);
        }
        else
        {
            set_tile(y, x, tile_corridor);
        }
    }

    return;
}


void v_corridor(int y1, int y2, int x)
{
    int y;
    tile_t t;

    if (y2 < y1)
    {
        y = y1;
        y1 = y2;
        y2 = y;
    }

    for (y = y1; y <= y2; y++)
    {
        if (//get_tile(y - 1, x) == tile_wall_ul ||
            get_tile(y - 1, x) == tile_wall_ll ||
            //get_tile(y + 1, x) == tile_wall_ul ||
            get_tile(y + 1, x) == tile_wall_ul)
        {
            set_tile(y, x, tile_corridor);
            set_tile(y, x - 1, tile_corridor);
        }

        if (//get_tile(y - 1, x) == tile_wall_ur ||
            get_tile(y - 1, x) == tile_wall_lr ||
            //get_tile(y + 1, x) == tile_wall_ur ||
            get_tile(y + 1, x) == tile_wall_ur)
        {
            set_tile(y, x, tile_corridor);
            set_tile(y, x + 1, tile_corridor);
        }

        t = get_tile(y, x);

        if (t == tile_wall_ul ||
            t == tile_wall_ll ||
            t == tile_wall_l)
        {
            set_tile(y, x - 1, tile_corridor);
        }
        else if (t == tile_wall_ur ||
                 t == tile_wall_lr ||
                 t == tile_wall_r)
        {
            set_tile(y, x + 1, tile_corridor);
        }
        else
        {
            set_tile(y, x, tile_corridor);
        }
    }

    return;
}
