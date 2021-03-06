#include <stdio.h>
#include <stdlib.h>
#include "dfs_maze_generator.h"
#include "grid_maze_topology.h"
#include "grid_maze_rooms_topology.h"
#include "grid_maze_ppm_print.h"
#include "grid_maze_random_rooms.h"

void render_tile_types(maze_t *maze, int *tile_types);
int overlaps(int start_i, int width_i, int start_j, int width_j);

int main(int argc, char **argv)
{

  int seed;
  if (argc > 1) {
    seed = atoi(argv[1]);
  } else {
    seed = 0;
  }

  srand(seed);

  const int HEIGHT = 50;
  const int WIDTH = 50;

  maze_tile_t **tiles;
  tiles = malloc(sizeof(maze_tile_t*) * HEIGHT);
  for (int j = 0; j < HEIGHT; j++) {
    tiles[j] = malloc(sizeof(maze_tile_t) * WIDTH);
    for (int i = 0; i < WIDTH; i++) {
      grid_maze_tile_t *t = malloc(sizeof(grid_maze_tile_t));
      t->i = i;
      t->j = j;
      t->type = no_passages;
      tiles[j][i].type = GRID_TILE;
      tiles[j][i].tile_data = t;
    }
  }

  const int ROOMS_LEN = 35;
  maze_room_t **rooms = malloc(sizeof(maze_room_t*) * ROOMS_LEN);

  int room_count = generate_rooms(HEIGHT, WIDTH, ROOMS_LEN, rooms);

  grid_maze_room_data_t *maze_data = malloc(sizeof(grid_maze_room_data_t));
  maze_data->height = HEIGHT;
  maze_data->width = WIDTH;
  maze_data->tiles = tiles;
  maze_data->rooms_length = room_count;
  maze_data->rooms = rooms;


  for (int r = 0; r < room_count; r++) {
    maze_room_t *room = rooms[r];
    for (int j = 0; j < room->height; j++) {
      for (int i = 0; i < room->width; i++) {
        maze_tile_t tile = tiles[j][i];
        tile.type = ROOM_TILE;
      }
    }
  }

  maze_t *maze = malloc(sizeof(maze_t));
  maze->maze_data = maze_data;

  generate_maze(maze, *get_random_grid_perimeter_tile, *get_grid_room_neighbors, *link_grid_room_tiles, *mark_start_grid_tile, *mark_end_grid_tile);

  int *tile_types = malloc(sizeof(int) * HEIGHT * WIDTH);
  render_tile_types(maze, tile_types);
  render_tiles(HEIGHT * WIDTH, WIDTH, tile_types);

  free(tile_types);
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      free(tiles[j][i].tile_data);
    }
    free(tiles[j]);
  }
  for (int r = 0; r < room_count; r++) {
    free(rooms[r]);
  }
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
