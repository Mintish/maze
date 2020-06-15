#include <stdlib.h>
#include "dfs_maze_generator.h"

#define MAX_FRONTIER 512

void pick_next_head(int frontier_length, maze_tile_t *frontier[])
{
  int pick = rand() % frontier_length;
  maze_tile_t *picked_tile = frontier[pick];
  frontier[pick] = frontier[frontier_length - 1];
  frontier[frontier_length - 1] = picked_tile;
}

void generate_maze(maze_t *maze,
                   maze_tile_t*(*get_start_tile)(maze_t*),
                   int(*get_neighbors)(maze_tile_t*, maze_tile_t*[], maze_t*),
                   void(*link_tiles)(maze_tile_t*, maze_tile_t*, maze_t*))
{
  maze_tile_t *frontier[MAX_FRONTIER];
  int frontier_p = 0;
  frontier[frontier_p] = (get_start_tile)(maze);

  int until = 0;
  while (frontier_p >= 0 && until <= 50000) {
    maze_tile_t *head = frontier[frontier_p];
    int frontier_length = frontier_p;
    frontier_p--;

    maze_tile_t *neighbors[MAX_NEIGHBORS];
    int neighbors_length = (get_neighbors)(head, neighbors, maze);

    if (neighbors_length > 0) {
      pick_next_head(neighbors_length, neighbors);

      maze_tile_t *t_h;
      maze_tile_t *t_c;
      for (int v = 0; v < neighbors_length; v++) {
        t_h = head;
        t_c = neighbors[v];
        frontier_p++;
        if (v < neighbors_length - 1) {
          // Re-add the current head as the back-track point
          frontier[frontier_p] = t_h;
        } else {
          frontier[frontier_p] = t_c;
        }
      }

      (link_tiles)(t_h, t_c, maze);
    }

    until++;
  }
}
