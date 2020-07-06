#include <stdio.h>
#include <stdlib.h>
#include "dfs_maze_generator.h"
#include "grid_maze_topology.h"
#include "grid_maze_rooms_topology.h"
#include "grid_maze_ppm_print.h"

void render_tile_types(maze_t *maze, int *tile_types);

int main(int argc, char **argv)
{

  int seed;
  if (argc > 1) {
    seed = atoi(argv[1]);
  } else {
    seed = 0;
  }

  srand(seed);

  const int height = 50;
  const int width = 50;

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

  maze_room_t *room0 = malloc(sizeof(maze_room_t));
  room0->start_i = 15;
  room0->start_j = 15;
  room0->height = 20;
  room0->width = 20;
  room0->marked_by = NULL;
  room0->flags = unvisited;
  maze_room_t *room1 = malloc(sizeof(maze_room_t));
  room1->start_i = 5;
  room1->start_j = 5;
  room1->height = 5;
  room1->width = 5;
  room1->marked_by = NULL;
  room1->flags = unvisited;

  maze_room_t **rooms = malloc(sizeof(maze_room_t*) * 2);
  rooms[0] = room0;
  //rooms[0] = room1;
  rooms[1] = room1;

  grid_maze_room_data_t *maze_data = malloc(sizeof(grid_maze_room_data_t));
  maze_data->height = height;
  maze_data->width = width;
  maze_data->tiles = tiles;
  maze_data->rooms_length = 1;
  maze_data->rooms = rooms;

  maze_t *maze = malloc(sizeof(maze_t));
  maze->maze_data = maze_data;

  generate_maze(maze, *get_random_grid_perimeter_tile, *get_grid_room_neighbors, *link_grid_room_tiles, *mark_start_grid_tile, *mark_end_grid_tile);

  int *tile_types = malloc(sizeof(int) * height * width);
  render_tile_types(maze, tile_types);
  render_tiles(height * width, width, tile_types);

  free(tile_types);
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      free(tiles[j][i].tile_data);
    }
    free(tiles[j]);
  }
  free(room0);
  free(room1);
  free(rooms);
  free(tiles);
  free(maze_data);
  free(maze);

  return 0;
}

void render_tile_types(maze_t *maze, int *tile_types)
{
  grid_maze_data_t *grid_maze = maze->maze_data;
  maze_tile_t **tiles = grid_maze->tiles;
  int height = grid_maze->height;
  int width = grid_maze->width;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      int c;
      grid_maze_tile_t *tile = tiles[j][i].tile_data;
      switch (tile->type) {
      case no_passages: // 0
        c = 0; break;
      case passage_up | passage_down | passage_left | passage_right: // 1
        c = 1; break;
      case passage_up | passage_right | passage_down: // 2
        c = 2; break;
      case passage_up | passage_left | passage_right: // 3
        c = 3; break;
      case passage_right | passage_down | passage_left: // 4
        c = 4; break;
      case passage_up | passage_down | passage_left: // 5
        c = 5; break;
      case passage_up | passage_right: // 6
        c = 6; break;
      case passage_up | passage_down: // 7
        c = 7; break;
      case passage_up | passage_left: // 8
        c = 8; break;
      case passage_right | passage_down: // 9
        c = 9; break;
      case passage_left | passage_right: // 10
        c = 10; break;
      case passage_left | passage_down: // 11
        c = 11; break;
      case passage_up: // 12
        c = 12; break;
      case passage_down: // 13
        c = 13; break;
      case passage_left: // 14
        c = 14; break;
      case passage_right: // 15
        c = 15; break;
      default:
        c = 0; break;
      }
      tile_types[i + (j * width)] = c;
    }
  }
}
