#define MAP_H 20
#define MAP_W 80

int current_depth;


typedef enum
{
  tile_void,
  tile_floor,
  tile_wall,
  tile_stair
} tile_t;


struct map_t
{
  tile_t map[MAP_H][MAP_W];
};


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


/* prototypes */
void shutdown();
void new_game();
void clear_map();
