#ifndef MAZE
#define MAZE

#define MAX_NEIGHBORS 10000

typedef enum { GRID_TILE, ROOM_TILE } tile_type;

typedef struct {
  void *maze_data;
} maze_t;

typedef struct {
  tile_type type;
  void *tile_data;
} maze_tile_t;

#endif
