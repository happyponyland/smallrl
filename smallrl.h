#define MAP_H 20
#define MAP_W 80

int current_depth;


enum tile_t
{
  tile_void,
  tile_floor,
  tile_wall,
  tile_stair
}


struct map_t
{
  tile_t map[MAP_H][MAP_W];
} map;


#define MOBS 40

enum mob_type_t
{
  mob_none,
  mob_player = '@',
  mob_newbie = 'n'
};

struct mob_t
{
  mob_type_t type;
  int y;
  int x;
} mob[MOBS];


/* prototypes */
void shutdown();
void new_game();
void clear_map();
