#include <stdlib.h>
#include "grid_maze_topology.h"

typedef struct {
  int height;
  int width;
} maze_dimensions;

int get_all_grid_neighbors(maze_tile_t *maze_tile, maze_tile_t *neighbors[], grid_maze_data_t *maze)
{
  maze_dimensions dimensions;
  dimensions.height = maze->height;
  dimensions.width = maze->width;
  maze_tile_t **tiles = maze->tiles;
  grid_maze_tile_t tile = *((grid_maze_tile_t*)maze_tile->tile_data);

  int neighbors_length;
  if (tile.i == 0 && tile.j == 0) { // top left
    neighbors_length = 2;
    maze_tile_t *right = &tiles[tile.j][tile.i + 1];
    maze_tile_t *down = &tiles[tile.j + 1][tile.i];
    neighbors[0] = right;
    neighbors[1] = down;
  } else if (tile.i == dimensions.width - 1 && tile.j == 0) { // top right
    neighbors_length = 2;
    maze_tile_t *down = &tiles[tile.j + 1][tile.i];
    maze_tile_t *left = &tiles[tile.j][tile.i - 1];
    neighbors[0] = down;
    neighbors[1] = left;
  } else if (tile.i == dimensions.width - 1 && tile.j == dimensions.height - 1) { // bottom right
    neighbors_length = 2;
    maze_tile_t *up = &tiles[tile.j - 1][tile.i];
    maze_tile_t *left = &tiles[tile.j][tile.i - 1];
    neighbors[0] = up;
    neighbors[1] = left;
  } else if (tile.i == 0 && tile.j == dimensions.height - 1) { // bottom left
    neighbors_length = 2;
    maze_tile_t *up = &tiles[tile.j - 1][tile.i];
    maze_tile_t *right = &tiles[tile.j][tile.i + 1];
    neighbors[0] = up;
    neighbors[1] = right;
  } else if (tile.i == 0) { // left
    neighbors_length = 3;
    maze_tile_t *up = &tiles[tile.j - 1][tile.i];
    maze_tile_t *right = &tiles[tile.j][tile.i + 1];
    maze_tile_t *down = &tiles[tile.j + 1][tile.i];
    neighbors[0] = up;
    neighbors[1] = right;
    neighbors[2] = down;
  } else if (tile.i == dimensions.width - 1) { // right
    neighbors_length = 3;
    maze_tile_t *up = &tiles[tile.j - 1][tile.i];
    maze_tile_t *down = &tiles[tile.j + 1][tile.i];
    maze_tile_t *left = &tiles[tile.j][tile.i - 1];
    neighbors[0] = up;
    neighbors[1] = down;
    neighbors[2] = left;
  } else if (tile.j == 0) { // top
    neighbors_length = 3;
    maze_tile_t *right = &tiles[tile.j][tile.i + 1];
    maze_tile_t *down = &tiles[tile.j + 1][tile.i];
    maze_tile_t *left = &tiles[tile.j][tile.i - 1];
    neighbors[0] = right;
    neighbors[1] = down;
    neighbors[2] = left;
  } else if (tile.j == dimensions.height - 1) { // bottom
    neighbors_length = 3;
    maze_tile_t *up = &tiles[tile.j - 1][tile.i];
    maze_tile_t *right = &tiles[tile.j][tile.i + 1];
    maze_tile_t *left = &tiles[tile.j][tile.i - 1];
    neighbors[0] = up;
    neighbors[1] = right;
    neighbors[2] = left;
  } else { // interior
    neighbors_length = 4;
    maze_tile_t *up = &tiles[tile.j - 1][tile.i];
    maze_tile_t *right = &tiles[tile.j][tile.i + 1];
    maze_tile_t *down = &tiles[tile.j + 1][tile.i];
    maze_tile_t *left = &tiles[tile.j][tile.i - 1];
    neighbors[0] = up;
    neighbors[1] = right;
    neighbors[2] = down;
    neighbors[3] = left;
  }

  return neighbors_length;
}

int get_valid_grid_neighbors(int neighbors_length, maze_tile_t *neighbors[], maze_tile_t *valid_neighbors[])
{
  int valid_neighbors_length = 0;
  for (int k = 0; k < neighbors_length; k++) {
    grid_maze_tile_t *neighbor = neighbors[k]->tile_data;
    if (neighbor->type == no_passages) {
      valid_neighbors[valid_neighbors_length] = neighbors[k];
      valid_neighbors_length++;
    }
  }
  return valid_neighbors_length;
}

int get_grid_neighbors(maze_tile_t *tile, maze_tile_t *neighbors[], maze_t* maze)
{
  maze_tile_t *all_neighbors[MAX_NEIGHBORS];
  grid_maze_data_t *grid_maze = maze->maze_data;
  int all_neighbors_length = get_all_grid_neighbors(tile, all_neighbors, grid_maze);
  int valid_neighbors_length = get_valid_grid_neighbors(all_neighbors_length, all_neighbors, neighbors); 
  return valid_neighbors_length;
}

maze_tile_t* get_grid_perimeter_tile(int index, grid_maze_data_t *maze)
{
  maze_dimensions dimensions;
  dimensions.height = maze->height;
  dimensions.width = maze->width;

  // Account for corners
  int width_d = dimensions.width - 1;
  int height_d = dimensions.height - 1;

  int perimeter = width_d * 2 + height_d * 2;
  int cw_index = index % perimeter;

  grid_maze_tile_t perimeter_tile;
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

  maze_tile_t *tile = &maze->tiles[perimeter_tile.j][perimeter_tile.i];

  return tile;
}

maze_tile_t* get_random_grid_perimeter_tile(maze_t *maze)
{
  grid_maze_data_t *grid_maze = maze->maze_data;
  int cw_index = rand();
  return get_grid_perimeter_tile(cw_index, grid_maze);
}

void link_grid_tiles(maze_tile_t *head, maze_tile_t *current, maze_t *maze)
{
  grid_maze_data_t *grid_maze = maze->maze_data;
  int height = grid_maze->height;
  int width = grid_maze->width;
  grid_maze_tile_t *t_h = head->tile_data;
  grid_maze_tile_t *t_c = current->tile_data;

  if (t_h->i == t_c->i && t_c->j < t_h->j) { // same column, current is higher up than head
    t_h->type |= passage_up;
    t_c->type |= passage_down;
  } else if (t_h->i == t_c->i && t_h->j < t_c->j) { // same column, current is lower than head
    t_h->type |= passage_down;
    t_c->type |= passage_up;
  } else if (t_h->j == t_c->j && t_c->i < t_h->i) { // same row, current is left of head
    t_h->type |= passage_left;
    t_c->type |= passage_right;
  } else if (t_h->j == t_c->j && t_h->i < t_c->i) { // same row, current is right of head
    t_h->type |= passage_right;
    t_c->type |= passage_left;
  }
}

