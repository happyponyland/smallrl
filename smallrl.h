#include <curses.h>
#include <stdlib.h>

#define MAP_H 20
#define MAP_W 80
#define MOBS 40

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
    int explored;
    int lit;
} tile_t;

typedef struct level_s
{
    tile_t map[MAP_H][MAP_W];
} level_t;

typedef enum color_e
{
    color_black,
    color_yellow,
    color_blue,
    color_red,
    color_green,
    color_cyan,
    color_magenta
} color_t;

typedef enum mob_type_e
{
    mob_none,
    mob_player = '@',
    mob_newbie = 'n'
} mob_type_t;

typedef struct mob_s
{
    mob_type_t type;
    int y;
    int x;
    int hp;
} mob_t;

int current_depth;
mob_t mob[MOBS];
mob_t * player;
level_t level;

/* smallrl.c */
void shutdown(void);

/* player.c */
int player_turn(void);
void explore(void);

/* level.c */
void clear_map(void);
int on_map(int y, int x);
void boring_level(int*, int*);
void build_room(int t, int l, int b, int r, int flags);
void set_tile(int y, int x, tile_type_t t);
tile_type_t get_tile_type(int y, int x);
void connect_rooms(int y1, int x1, int y2, int x2);
void h_corridor(int y, int x1, int x2);
void v_corridor(int y1, int y2, int x);
void set_tile_flags_by_type(tile_t * tile, tile_type_t tile_type);

/* game.c */
void new_game(void);
int move_mob(mob_t *, int x_speed, int y_speed);
int play(void);
void enemy_turn(int id);

/* ui.c */
void draw_map(void);
void draw_stats(void);
int prompt_yn(char *);
void print_msg(char *);
void clear_msg(void);
void wait(void);
