#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int i;
  int j;
} maze_tile;

typedef struct {
  int height;
  int width;
} maze_dimensions;

typedef enum {
  no_passages = 0,
  passage_up = 1,
  passage_right = 2,
  passage_down = 4,
  passage_left = 8,
} tile_type;

typedef enum {
  explored = 1
} flag;

typedef struct {
  tile_type type;
  flag flag;
} tile;

int get_neighbors(maze_tile tile, maze_tile neighbors[], maze_dimensions dimensions)
{
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

maze_tile get_random_perimeter_tile(maze_dimensions dimensions)
{
  int cw_index = rand();
  return get_perimeter_tile(cw_index, dimensions);
}

int are_tiles_equal(maze_tile tile1, maze_tile tile2)
{
  int result;
  if (tile1.i == tile2.i && tile1.j == tile2.j) {
    result = 1;
  } else {
    result = 0;
  }
  return result;
}

int valid_neighbors(int neighbors_length, maze_tile neighbors[], int n, int m, tile tiles[n][m], maze_tile valid_neighbors[])
{
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

void pick_random_next_head(maze_tile frontier[], int frontier_length)
{
  if (frontier_length == 0) {
    return;
  }
  int pick = rand() % frontier_length;
  maze_tile picked_tile = frontier[pick];
  frontier[pick] = frontier[frontier_length - 1];
  frontier[frontier_length - 1] = picked_tile;
}

void render_tiles(int height, int width, tile tiles[height][width])
{
  int character_height = height * 2 + 1;
  int character_width = width * 2 + 1;

  for (int i = 0; i < character_height; i++) {
    for (int j = 0; j < character_width; j++) {
      int t_i = (width * (j + 0)) / character_width;
      int t_i_l = (width * (j - 1)) / character_width;
      int t_i_r = (width * (j + 1)) / character_width;
      int t_j = (height * (i + 0)) / character_height;
      int t_j_u = (height * (i - 1)) / character_height;
      int t_j_d = (height * (i + 1)) / character_height;

      if (i % 2 == 0 && j % 2 == 0) {
        printf("#");
      } else if (i % 2 == 0) {
        if (t_j_u < t_j && t_j_u > 0 && (tiles[t_j][t_i].type & passage_up) == passage_up) {
          printf("i");
        } else if (t_j_u < t_j && t_j > 0 && (tiles[t_j - 1][t_i].type & passage_down) == passage_down) {
          printf("j");
        } else if (t_j < t_j_d && t_j_d < height && (tiles[t_j][t_i].type & passage_down) == passage_down) {
          printf(".");
        } else if (t_j < t_j_d && t_j + 1 < height && (tiles[t_j + 1][t_i].type & passage_up) == passage_up) {
          printf(".");
        } else {
          printf("#");
        }
      } else if (j % 2 == 0) {
        if (t_i_l < t_i && t_i_l > 0 && (tiles[t_j][t_i].type & passage_left) == passage_left) {
          printf("a");
        } else if (t_i_l < t_i && t_i > 0 && (tiles[t_j][t_i - 1].type & passage_right) == passage_right) {
          printf("b");
        } else if (t_i < t_i_r && t_i_r < width && (tiles[t_j][t_i].type & passage_right) == passage_right) {
          printf(".");
        } else if (t_i < t_i_r && t_i + 1 < width && (tiles[t_j][t_i + 1].type & passage_left) == passage_left) {
          printf(".");
        } else {
          printf("#");
        }
      } else {
        printf(".");
      }
    }
    printf("\n");
  }
}
int main(int argc, char **argv)
{

  int seed;
  if (argc > 1) {
    seed = atoi(argv[1]);
  } else {
    seed = 0;
  }

  srand(seed);

  const int height = 15;
  const int width = 25;
  const int maze_size = height * width;
  const int neighbors_max_length = 4;

  maze_tile neighbors[neighbors_max_length];
  maze_dimensions dimensions;
  dimensions.height = height;
  dimensions.width = width;

  maze_tile frontier[maze_size];

  maze_tile start_tile = get_random_perimeter_tile(dimensions);

  frontier[0] = start_tile;
  int frontier_p = 0;

  tile tiles[height][width];
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      tile t;
      t.type = no_passages;
      t.flag = 0;
      tiles[j][i] = t;
    }
  }

  int until = 0;
  while (frontier_p >= 0 && until <= 50000) {
    maze_tile head = frontier[frontier_p];

    tiles[head.j][head.i].flag = explored;
    frontier_p--;

    int frontier_length = frontier_p + 1;
    int neighbors_length = get_neighbors(head, neighbors, dimensions);
    maze_tile valid_unseen_neighbors[neighbors_max_length];
    int valid_unseen_neighbors_length = valid_neighbors(neighbors_length, neighbors, height, width, tiles, valid_unseen_neighbors);

    pick_random_next_head(valid_unseen_neighbors, valid_unseen_neighbors_length);

    maze_tile t_h;
    maze_tile t_c;
    for (int v = 0; v < valid_unseen_neighbors_length; v++) {
      t_h = head;
      t_c = valid_unseen_neighbors[v];
      frontier_p++;
      if (v < valid_unseen_neighbors_length - 1) {
        // Re-add the current head as the back-track point
        frontier[frontier_p] = t_h;
      } else {
        frontier[frontier_p] = t_c;
      }
    }

    // Link the current head with the new head
    if (valid_unseen_neighbors_length > 0) {
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

    until++;
  }
  
  render_tiles(height, width, tiles);


  return 0;
}
