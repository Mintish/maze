#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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

int left_disjunction(maze_tile left_tiles[], int left_length, maze_tile right_tiles[], int right_length, maze_tile disjunction[])
{
  int disjunction_length = 0;
  for (int l = 0; l < left_length; l++) {
    int found = 0;
    for (int r = 0; r < right_length; r++) {
      if (are_tiles_equal(left_tiles[l], right_tiles[r])) {
        found = 1;
      }
    }
    if (!found) {
      disjunction[disjunction_length] = left_tiles[l];
      disjunction_length++;
    }
  }
  return disjunction_length;
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

void debugf(const char *format, ...)
{
  // This doesn't quite work...
  #if VERBOSE 
  va_list rest;
  va_start(rest, format);
  printf(format, rest);
  va_end(rest);
  #endif
}

void print_tiles(char *label, maze_tile tiles[], int tiles_length)
{
  debugf("%s: ", label);
  for (int i = 0; i < tiles_length; i++) {
    debugf("(%d, %d) ", tiles[i].i, tiles[i].j);
  }
  debugf("\n");
}

void render_tiles(int height, int width, tile_type tiles[10][10])
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

  const int neighbors_max_length = 4;
  maze_tile neighbors[neighbors_max_length];
  maze_dimensions dimensions;

  dimensions.height = 10;
  dimensions.width = 10;

  const int frontier_length = 100;
  maze_tile frontier[frontier_length];
  maze_tile seen[frontier_length];
  maze_path paths[100];
  int paths_length = 0;
  int maze_edges[100][100];
  for (int i; i < 100; i++) {
    for (int j; j < 100; j++) {
      maze_edges[i][j] = 0;
    }
  }

  maze_tile start_tile = get_random_perimeter_tile(dimensions);

  frontier[0] = start_tile;
  int frontier_p = 0;
  int seen_length = 0;

  maze_tile valid_neighbors[neighbors_max_length];

  int until = 0;
  while (frontier_p >= 0 && until <= 50000) {
    maze_tile head = frontier[frontier_p];
    seen[seen_length] = head;
    seen_length++;
    frontier_p--;
    int frontier_length = frontier_p + 1;
    debugf("head: (%d, %d)\n", head.i, head.j);
    print_tiles("frontier", frontier, frontier_length);
    print_tiles("seen", seen, seen_length);
    int neighbors_length = get_neighbors(head, neighbors, dimensions);
    int valid_neighbors_length = left_disjunction(neighbors, neighbors_length, frontier, frontier_length, valid_neighbors);
    maze_tile valid_unseen_neighbors[neighbors_max_length];
    int valid_unseen_neighbors_length = left_disjunction(valid_neighbors, valid_neighbors_length, seen, seen_length, valid_unseen_neighbors);
    print_tiles("valid_neighbors", valid_neighbors, valid_neighbors_length);
    print_tiles("valid_unseen_neighbors", valid_unseen_neighbors, valid_unseen_neighbors_length);
    pick_random_next_head(valid_unseen_neighbors, valid_unseen_neighbors_length);
    print_tiles("valid_unseen_neighbors random pick", valid_unseen_neighbors, valid_unseen_neighbors_length);
    for (int v = 0; v < valid_unseen_neighbors_length; v++) {
      frontier_p++;
      frontier[frontier_p] = valid_unseen_neighbors[v];
      maze_path path;
      path.t1 = head;
      path.t2 = frontier[frontier_p];
      paths[paths_length] = path;
      maze_edges[path.t1.i + dimensions.height * path.t1.j][path.t2.i + dimensions.height * path.t2.j] = 1;
      maze_edges[path.t2.i + dimensions.height * path.t2.j][path.t1.i + dimensions.height * path.t1.j] = 1;
      paths_length++;
   }
    print_tiles("updated frontier", frontier, frontier_p);
    until++;
  }
  debugf("until = %d\n", until);

  /*
  printf("digraph G {\n");
  for (int p = 0; p < paths_length; p++) {
    printf("\tmaze_tile_%d_%d -> maze_tile_%d_%d;\n", paths[p].t1.i, paths[p].t1.j, paths[p].t2.i, paths[p].t2.j);
  }
  printf("}\n");
  */

  tile_type tiles[10][10];
  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++)
      tiles[j][i] = no_passages;

  for (int p = 0; p < paths_length; p++) {
    maze_path path = paths[p];
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

  render_tiles(10, 10, tiles);

  return 0;
}
