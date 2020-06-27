#include <stdio.h>
#include <stdlib.h>
#include "dfs_maze_generator.h"
#include "grid_maze_topology.h"

void render_tiles(maze_t *maze);
void render_tiles_small(maze_t *maze);

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
  const int width = 35;

  maze_tile_t **tiles;
  tiles = malloc(sizeof(maze_tile_t*) * height);
  for (int j = 0; j < height; j++) {
    tiles[j] = malloc(sizeof(maze_tile_t) * width);
    for (int i = 0; i < width; i++) {
      grid_maze_tile_t *t = malloc(sizeof(grid_maze_tile_t));
      t->i = i;
      t->j = j;
      t->type = no_passages;
      tiles[j][i].tile_data = t;
    }
  }

  grid_maze_data_t *maze_data = malloc(sizeof(grid_maze_data_t));
  maze_data->height = height;
  maze_data->width = width;
  maze_data->tiles = tiles;

  maze_t *maze = malloc(sizeof(maze_t));
  maze->maze_data = maze_data;

  generate_maze(maze, *get_random_grid_perimeter_tile, *get_grid_neighbors, *link_grid_tiles, *mark_start_grid_tile, *mark_end_grid_tile);

  render_tiles_small(maze);
  render_tiles(maze);

  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      free(tiles[j][i].tile_data);
    }
    free(tiles[j]);
  }
  free(tiles);
  free(maze_data);
  free(maze);

  return 0;
}

void render_tiles(maze_t *maze)
{
  grid_maze_data_t *grid_maze = maze->maze_data;
  maze_tile_t **tiles = grid_maze->tiles;
  int height = grid_maze->height;
  int width = grid_maze->width;
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
        if (t_j < t_j_d && t_j_d < height && (((grid_maze_tile_t*)tiles[t_j][t_i].tile_data)->type & passage_down) == passage_down) {
          printf(".");
        } else if (t_j < t_j_d && t_j + 1 < height && (((grid_maze_tile_t*)tiles[t_j + 1][t_i].tile_data)->type & passage_up) == passage_up) {
          printf(".");
        } else {
          printf("#");
        }
      } else if (j % 2 == 0) {
        if (t_i < t_i_r && t_i_r < width && (((grid_maze_tile_t*)tiles[t_j][t_i].tile_data)->type & passage_right) == passage_right) {
          printf(".");
        } else if (t_i < t_i_r && t_i + 1 < width && (((grid_maze_tile_t*)tiles[t_j][t_i + 1].tile_data)->type & passage_left) == passage_left) {
          printf(".");
        } else {
          printf("#");
        }
      } else {
        grid_maze_tile_t *tile = tiles[t_j][t_i].tile_data;
        if ((tile->type & start) == start) {
          printf("S");
        } else if ((tile->type & end) == end) {
          printf("E");
        } else {
          printf(".");
        }
      }
    }
    printf("\n");
  }
}

void render_tiles_small(maze_t *maze)
{

  grid_maze_data_t *grid_maze = maze->maze_data;
  maze_tile_t **tiles = grid_maze->tiles;
  int height = grid_maze->height;
  int width = grid_maze->width;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      char *c;
      grid_maze_tile_t *tile = tiles[j][i].tile_data;
      switch (tile->type) {
      case no_passages:
        c = " ";
        break;
      case passage_up | passage_down | passage_left | passage_right:
        c = "╬"; break;
      case passage_up | passage_right | passage_down:
        c = "╠"; break;
      case passage_up | passage_left | passage_right:
        c = "╩"; break;
      case passage_right | passage_down | passage_left:
        c = "╦"; break;
      case passage_up | passage_down | passage_left:
        c = "╣"; break;
      case passage_up | passage_right:
        c = "╚"; break;
      case passage_up | passage_down:
        c = "║"; break;
      case passage_up | passage_left:
        c = "╝"; break;
      case passage_right | passage_down:
        c = "╔"; break;
      case passage_left | passage_right:
        c = "═"; break;
      case passage_left | passage_down:
        c = "╗"; break;
      case passage_up:
        c = "╨";
        break;
      case passage_down:
        c = "╥";
        break;
      case passage_left:
        c = "╡";
        break;
      case passage_right:
        c = "╞";
        break;
      default: {
        char str[12];
        sprintf(str, "%d", tile->type); break;
        c = str;
      }
      }
      printf("%s", c);
    }
    printf("\n");
  }
}
