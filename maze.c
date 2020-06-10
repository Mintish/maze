#include <stdlib.h>
#include "maze.h"

#define MAX_NEIGHBORS 8
#define MAX_FRONTIER 512

typedef struct {
  int height;
  int width;
} maze_dimensions;

int get_all_neighbors(maze_tile tile, maze_tile neighbors[], maze *maze)
{
  maze_dimensions dimensions;
  dimensions.height = maze->height;
  dimensions.width = maze->width;
  maze_tile left;
  maze_tile right;
  maze_tile up;
  maze_tile down;

  left.i = tile.i - 1;
  left.j = tile.j;
  right.i = tile.i + 1;
  right.j = tile.j;
  up.i = tile.i;
  up.j = tile.j - 1;
  down.i = tile.i;
  down.j = tile.j + 1;

  int neighbors_length;
  if (tile.i == 0 && tile.j == 0) { // top left
    neighbors_length = 2;
    neighbors[0] = right;
    neighbors[1] = down;
  } else if (tile.i == dimensions.width - 1 && tile.j == 0) { // top right
    neighbors_length = 2;
    neighbors[0] = down;
    neighbors[1] = left;
  } else if (tile.i == dimensions.width - 1 && tile.j == dimensions.height - 1) { // bottom right
    neighbors_length = 2;
    neighbors[0] = up;
    neighbors[1] = left;
  } else if (tile.i == 0 && tile.j == dimensions.height - 1) { // bottom left
    neighbors_length = 2;
    neighbors[0] = up;
    neighbors[1] = right;
  } else if (tile.i == 0) { // left
    neighbors_length = 3;
    neighbors[0] = up;
    neighbors[1] = right;
    neighbors[2] = down;
  } else if (tile.i == dimensions.width - 1) { // right
    neighbors_length = 3;
    neighbors[0] = up;
    neighbors[1] = down;
    neighbors[2] = left;
  } else if (tile.j == 0) { // top
    neighbors_length = 3;
    neighbors[0] = right;
    neighbors[1] = down;
    neighbors[2] = left;
  } else if (tile.j == dimensions.height - 1) { // bottom
    neighbors_length = 3;
    neighbors[0] = up;
    neighbors[1] = right;
    neighbors[2] = left;
  } else { // interior
    neighbors_length = 4;
    neighbors[0] = up;
    neighbors[1] = right;
    neighbors[2] = down;
    neighbors[3] = left;
  }

  return neighbors_length;
}

int get_valid_neighbors(int neighbors_length, maze_tile neighbors[], maze *maze, maze_tile valid_neighbors[])
{
  tile **tiles = maze->tiles;
  int valid_neighbors_length = 0;
  for (int k = 0; k < neighbors_length; k++) {
    maze_tile neighbor = neighbors[k];
    if (tiles[neighbor.j][neighbor.i].type == no_passages) {
      valid_neighbors[valid_neighbors_length] = neighbor;
      valid_neighbors_length++;
    }
  }
  return valid_neighbors_length;
}

int get_grid_neighbors(maze_tile tile, maze_tile neighbors[], maze* maze)
{
  maze_tile all_neighbors[MAX_NEIGHBORS];
  int all_neighbors_length = get_all_neighbors(tile, all_neighbors, maze);
  int valid_neighbors_length = get_valid_neighbors(all_neighbors_length, all_neighbors, maze, neighbors); 
  return valid_neighbors_length;
}

maze_tile get_perimeter_tile(int index, maze_dimensions dimensions)
{
  // Account for corners
  int width_d = dimensions.width - 1;
  int height_d = dimensions.height - 1;

  int perimeter = width_d * 2 + height_d * 2;
  int cw_index = index % perimeter;

  maze_tile perimeter_tile;
  if (cw_index < width_d) { // top
    perimeter_tile.i = cw_index;
    perimeter_tile.j = 0;
  } else if (cw_index < width_d + height_d) { // right
    perimeter_tile.i = width_d;
    perimeter_tile.j = cw_index - width_d;
  } else if (cw_index < width_d * 2 + height_d) { // bottom
    perimeter_tile.i = cw_index - (width_d + height_d);
    perimeter_tile.j = height_d;
  } else { // left
    perimeter_tile.i = 0;
    perimeter_tile.j = cw_index - (width_d * 2 + height_d);
  }

  return perimeter_tile;
}

maze_tile get_random_grid_perimeter_tile(maze *maze)
{
  maze_dimensions dimensions;
  dimensions.height = maze->height;
  dimensions.width = maze->width;
  int cw_index = rand();
  return get_perimeter_tile(cw_index, dimensions);
}

void link_grid_tiles(maze_tile head, maze_tile current, maze *maze)
{
  tile **tiles = maze->tiles;
  int height = maze->height;
  int width = maze->width;
  maze_tile t_h = head;
  maze_tile t_c = current;

  if (t_h.i == t_c.i && t_c.j < t_h.j) { // same column, current is higher up than head
    tiles[t_h.j][t_h.i].type |= passage_up;
    tiles[t_c.j][t_c.i].type |= passage_down;
  } else if (t_h.i == t_c.i && t_h.j < t_c.j) { // same column, current is lower than head
    tiles[t_h.j][t_h.i].type |= passage_down;
    tiles[t_c.j][t_c.i].type |= passage_up;
  } else if (t_h.j == t_c.j && t_c.i < t_h.i) { // same row, current is left of head
    tiles[t_h.j][t_h.i].type |= passage_left;
    tiles[t_c.j][t_c.i].type |= passage_right;
  } else if (t_h.j == t_c.j && t_h.i < t_c.i) { // same row, current is right of head
    tiles[t_h.j][t_h.i].type |= passage_right;
    tiles[t_c.j][t_c.i].type |= passage_left;
  }
}

void pick_next_head(int frontier_length, maze_tile frontier[])
{
  if (frontier_length == 0) {
    return;
  }
  int pick = rand() % frontier_length;
  maze_tile picked_tile = frontier[pick];
  frontier[pick] = frontier[frontier_length - 1];
  frontier[frontier_length - 1] = picked_tile;
}

void generate_maze(maze *maze_data, maze_tile(*get_start_tile)(maze*), int(*get_neighbors)(maze_tile, maze_tile[], maze*), void(*link_tiles)(maze_tile, maze_tile, maze*))
{
  maze_tile frontier[MAX_FRONTIER];
  int frontier_p = 0;
  frontier[frontier_p] = (get_start_tile)(maze_data);

  int until = 0;
  while (frontier_p >= 0 && until <= 50000) {
    maze_tile head = frontier[frontier_p];
    int frontier_length = frontier_p;
    frontier_p--;

    maze_tile neighbors[MAX_NEIGHBORS];
    int neighbors_length = (get_neighbors)(head, neighbors, maze_data);
    pick_next_head(neighbors_length, neighbors);

    maze_tile t_h;
    maze_tile t_c;
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

    // Link the current head with the new head
    if (neighbors_length > 0) {
      (link_tiles)(t_h, t_c, maze_data);
    }

    until++;
  }
}
