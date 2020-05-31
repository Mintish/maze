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

typedef struct {
  maze_tile t1;
  maze_tile t2;
} maze_path;

typedef enum {
  no_passages = 0,
  passage_up = 1,
  passage_right = 2,
  passage_down = 4,
  passage_left = 8,
} tile_type;

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
  // Subtract `4` or we count the corners twice.
  int perimeter = dimensions.width * 2 + dimensions.height * 2 - 4;
  int cw_index = index % perimeter;
  int side = (4 * cw_index) / perimeter;

  // Offsets also need to account for corners.
  int offset;
  maze_tile perimeter_tile;
  if (side == 0) { // top
    offset = 0;
    perimeter_tile.i = cw_index - offset;
    perimeter_tile.j = 0;
  } else if (side == 1) { // right
    offset = dimensions.width - 1;
    perimeter_tile.i = dimensions.width - 1;
    perimeter_tile.j = cw_index - offset;
  } else if (side == 2) { // bottom
    offset = dimensions.width + dimensions.height - 2;
    perimeter_tile.i = cw_index - offset;
    perimeter_tile.j = dimensions.height - 1;
  } else { // left
    offset = dimensions.width * 2 + dimensions.height - 4;
    perimeter_tile.i = 0;
    perimeter_tile.j = cw_index - offset;
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

int valid_neighbors(int neighbors_length, maze_tile neighbors[], int n, int m, tile_type tiles[n][m], maze_tile valid_neighbors[])
{
  int valid_neighbors_length = 0;
  for (int k = 0; k < neighbors_length; k++) {
    maze_tile neighbor = neighbors[k];
    if (tiles[neighbor.j][neighbor.i] == no_passages) {
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

void render_tiles(int height, int width, tile_type tiles[height][width])
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
        if (t_j_u < t_j && t_j_u > 0 && (tiles[t_j][t_i] & passage_up) == passage_up) {
          printf("i");
        } else if (t_j_u < t_j && t_j > 0 && (tiles[t_j - 1][t_i] & passage_down) == passage_down) {
          printf("j");
        } else if (t_j < t_j_d && t_j_d < height && (tiles[t_j][t_i] & passage_down) == passage_down) {
          printf(".");
        } else if (t_j < t_j_d && t_j + 1 < height && (tiles[t_j + 1][t_i] & passage_up) == passage_up) {
          printf(".");
        } else {
          printf("#", t_j);
        }
      } else if (j % 2 == 0) {
        if (t_i_l < t_i && t_i_l > 0 && (tiles[t_j][t_i] & passage_left) == passage_left) {
          printf("a");
        } else if (t_i_l < t_i && t_i > 0 && (tiles[t_j][t_i - 1] & passage_right) == passage_right) {
          printf("b");
        } else if (t_i < t_i_r && t_i_r < width && (tiles[t_j][t_i] & passage_right) == passage_right) {
          printf(".");
        } else if (t_i < t_i_r && t_i + 1 < width && (tiles[t_j][t_i + 1] & passage_left) == passage_left) {
          printf(".");
        } else {
          printf("#");
        }
      } else {
        printf(".", t_j);
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

  const int height = 5;
  const int width = 5;
  const int maze_size = height * width;
  const int neighbors_max_length = 4;

  maze_tile neighbors[neighbors_max_length];
  maze_dimensions dimensions;
  dimensions.height = height;
  dimensions.width = width;

  maze_tile frontier[maze_size];
  maze_tile seen[maze_size];
  int seen_length = 0;

  maze_tile start_tile = get_random_perimeter_tile(dimensions);

  frontier[0] = start_tile;
  int frontier_p = 0;

  tile_type tiles[height][width];
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
      tiles[j][i] = no_passages;

  int until = 0;
  while (frontier_p >= 0 && until <= 50000) {
    maze_tile head = frontier[frontier_p];
    seen[seen_length] = head;
    seen_length++;
    frontier_p--;
    int frontier_length = frontier_p + 1;
    int neighbors_length = get_neighbors(head, neighbors, dimensions);
    maze_tile valid_unseen_neighbors[neighbors_max_length];
    int valid_unseen_neighbors_length = valid_neighbors(neighbors_length, neighbors, height, width, tiles, valid_unseen_neighbors);

    pick_random_next_head(valid_unseen_neighbors, valid_unseen_neighbors_length);
    for (int v = 0; v < valid_unseen_neighbors_length; v++) {
      frontier_p++;
      frontier[frontier_p] = valid_unseen_neighbors[v];
      maze_path path;
      path.t1 = head;
      path.t2 = frontier[frontier_p];

      if (path.t1.i == path.t2.i && path.t2.j < path.t1.j) {
        tiles[path.t1.j][path.t1.i] |= passage_up;
        tiles[path.t2.j][path.t2.i] |= passage_down;
      } else if (path.t1.i == path.t2.i && path.t1.j < path.t2.j) {
        tiles[path.t1.j][path.t1.i] |= passage_down;
        tiles[path.t2.j][path.t2.i] |= passage_up;
      } else if (path.t1.j == path.t2.j && path.t2.i < path.t1.i) {
        tiles[path.t1.j][path.t1.i] |= passage_left;
        tiles[path.t2.j][path.t2.i] |= passage_right;
      } else if (path.t1.j == path.t2.j && path.t1.i < path.t2.i) {
        tiles[path.t1.j][path.t1.i] |= passage_right;
        tiles[path.t2.j][path.t2.i] |= passage_left;
      }
   }
    until++;
  }

  render_tiles(height, width, tiles);

  return 0;
}
