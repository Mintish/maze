#include "grid_maze_topology.h"
#ifndef GRID_ROOM
#define GRID_ROOM

typedef enum { unvisited = 0, visited = 1 } room_flags;

typedef struct {
  int start_j;
  int start_i;
  int height;
  int width;
  room_flags flags;
  grid_maze_tile_t *marked_by;
} maze_room_t;

typedef struct {
  int height;
  int width;
  maze_tile_t **tiles;
  int rooms_length;
  maze_room_t **rooms;
} grid_maze_room_data_t;

int get_grid_room_neighbors(maze_tile_t *tie, maze_tile_t *neighbors[], maze_t *maze);

void link_grid_room_tiles(maze_tile_t *head, maze_tile_t *current, maze_t *maze);
#endif
