#include "maze.h"

#ifndef GRID_TOPOLOGY
#define GRID_TOPOLOGY

typedef enum {
  no_passages = 0,
  passage_up = 1,
  passage_right = 2,
  passage_down = 4,
  passage_left = 8,
  start = 16,
  end = 32,
} grid_tile_type;

typedef struct {
  int i;
  int j;
  grid_tile_type type;
} grid_maze_tile_t;

typedef struct {
  int height;
  int width;
  maze_tile_t **tiles;
} grid_maze_data_t;

maze_tile_t* get_random_grid_perimeter_tile(maze_t *maze);

int get_grid_neighbors(maze_tile_t *tile, maze_tile_t *neighbors[], maze_t* maze);

int get_valid_grid_neighbors(int neighbors_length, maze_tile_t *neighbors[], maze_tile_t *valid_neighbors[]);

int get_all_grid_neighbors(maze_tile_t *maze_tile, maze_tile_t *neighbors[], grid_maze_data_t *maze);

void link_grid_tiles(maze_tile_t *head, maze_tile_t *current, maze_t *maze);

void mark_start_grid_tile(maze_tile_t *tile);

void mark_end_grid_tile(maze_tile_t *tile);

#endif
