#include <stdlib.h>
#include <stdio.h>
#include "dfs_maze_generator.h"

#define MAX_FRONTIER 100

maze_tile_t* pick_next_head(int frontier_length, maze_tile_t *frontier[])
{
  int pick = rand() % frontier_length;
  maze_tile_t *picked_tile = frontier[pick];
  return picked_tile;
}

void generate_maze(maze_t *maze,
                   maze_tile_t*(*get_start_tile)(maze_t*),
                   int(*get_neighbors)(maze_tile_t*, maze_tile_t*[], maze_t*),
                   void(*link_tiles)(maze_tile_t*, maze_tile_t*, maze_t*),
                   void(*mark_start_tile)(maze_tile_t*),
                   void(*mark_end_tile)(maze_tile_t*))
{
  maze_tile_t *frontier[MAX_FRONTIER];
  int frontier_p = 0;
  maze_tile_t *start_tile = get_start_tile(maze);
  frontier[frontier_p] = start_tile;
  mark_start_tile(start_tile);

  int max_depth = 0;
  maze_tile_t *deepest_tile;
  while (frontier_p >= 0) {
    maze_tile_t *head = frontier[frontier_p];

    maze_tile_t *neighbors[MAX_NEIGHBORS];
    int neighbors_length = get_neighbors(head, neighbors, maze);

    if (neighbors_length > 0) {
      maze_tile_t *new_head = pick_next_head(neighbors_length, neighbors);
      frontier_p++;
      frontier[frontier_p] = new_head;
      link_tiles(head, new_head, maze);
    } else if (frontier_p > max_depth) {
      deepest_tile = head;
      max_depth = frontier_p;
      frontier_p--;
    } else {
      frontier_p--;
    }
  }
  mark_end_tile(deepest_tile);
}
