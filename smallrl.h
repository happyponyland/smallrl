#include <curses.h>
#include <stdlib.h>

#define MAP_H 20
#define MAP_W 80

int current_depth;


typedef enum
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
} tile_t;


struct level_t
{
  tile_t map[MAP_H][MAP_W];
} level;


enum color_t
{
  color_black,
  color_yellow,
  color_blue,
  color_red,
  color_green,
  color_cyan,
  color_magenta
};
typedef enum color_t color_t;


#define MOBS 40

typedef enum
{
  mob_none,
  mob_player = '@',
  mob_newbie = 'n'
} mob_type_t;

struct mob_t
{
  mob_type_t type;
  int y;
  int x;
  int hp;
} mob[MOBS];


/* smallrl.c */
void shutdown(void);
void map_test(void);

/* player.c */
int player_turn(void);

/* level.c */
void clear_map(void);
int on_map(int y, int x);
void boring_level(void);
void build_room(int t, int l, int b, int r, int flags);
void set_tile(int y, int x, tile_t t);
tile_t get_tile(int y, int x);
void connect_rooms(int y1, int x1, int y2, int x2);
void h_corridor(int y, int x1, int x2);
void v_corridor(int y1, int y2, int x);

/* game.c */
void new_game(void);
int move_mob(int id, int y_speed, int x_speed);
int play(void);
void enemy_turn(int id);

/* ui.c */
void draw_map(void);
void draw_stats(void);
int prompt_yn(char * m);
void print_msg(char * m);
void clear_msg(void);
void wait(void);

