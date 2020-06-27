#include "maze.h"

#ifndef DFS_GENERATOR
#define DFS_GENERATOR

void generate_maze(maze_t *maze,
                   maze_tile_t*(*get_start_tile)(maze_t*),
                   int(*get_neighbors)(maze_tile_t*, maze_tile_t*[], maze_t*),
                   void(*link_tiles)(maze_tile_t*, maze_tile_t*, maze_t*),
                   void(*mark_start_tile)(maze_tile_t*),
                   void(*mark_end_tile)(maze_tile_t*));

#endif
