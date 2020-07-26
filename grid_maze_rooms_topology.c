#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "grid_maze_topology.h"
#include "grid_maze_rooms_topology.h"


maze_room_t *get_intersecting_room(maze_tile_t *tile, maze_t *maze);
int get_room_perimiter_tiles(maze_room_t *room, maze_t *maze, maze_tile_t *permiter_tiles[]);
void print_maze_room(maze_room_t *room);
void print_maze_tile(maze_tile_t *tile);

int get_grid_room_neighbors(maze_tile_t *tile, maze_tile_t *neighbors[], maze_t *maze)
{
  grid_maze_room_data_t *room_maze = maze->maze_data;
  grid_maze_data_t *grid_maze = maze->maze_data;
  maze_tile_t *all_neighbors[MAX_NEIGHBORS];
  grid_maze_tile_t *t = tile->tile_data;
  //printf("t: %d, %d\n", t->j, t->i);
  int all_neighbors_length = get_all_grid_neighbors(tile, all_neighbors, grid_maze);
  int non_intersecting_neighbors_length = 0;
  maze_tile_t *non_intersecting_neighbors[MAX_NEIGHBORS];
  for (int n = 0; n < all_neighbors_length; n++) {
    grid_maze_tile_t *neighbor = all_neighbors[n]->tile_data;
    maze_room_t *room_hit = get_intersecting_room(all_neighbors[n], maze);
    if (room_hit != NULL && room_hit->flags == unvisited) {
      non_intersecting_neighbors[non_intersecting_neighbors_length] = all_neighbors[n];
      non_intersecting_neighbors_length++;
      room_hit->flags = visited;
      room_hit->marked_by = tile->tile_data;
    } else if (room_hit == NULL || room_hit->marked_by == tile->tile_data) {
      non_intersecting_neighbors[non_intersecting_neighbors_length] = all_neighbors[n];
      non_intersecting_neighbors_length++;
    }
  }

  maze_room_t *tile_room = get_intersecting_room(tile, maze);
  if (tile_room != NULL) {
    maze_tile_t *perimeter_tiles[MAX_NEIGHBORS];
    int perimeter_tiles_length = get_room_perimiter_tiles(tile_room, maze, perimeter_tiles);
    //printf("r: %d, %d; %d, %d\n", tile_room->start_j, tile_room->start_i, tile_room->height, tile_room->width);
    for (int p = 0; p < perimeter_tiles_length; p++) {
      non_intersecting_neighbors[non_intersecting_neighbors_length] = perimeter_tiles[p];
      non_intersecting_neighbors_length++;
      grid_maze_tile_t *rt = (grid_maze_tile_t*)perimeter_tiles[p]->tile_data;
      //printf("rt: %d, %d\n", rt->j, rt->i);
    }
    //printf("\n");
  }

  int neighbors_length = get_valid_grid_neighbors(non_intersecting_neighbors_length, non_intersecting_neighbors, neighbors);

  return neighbors_length;
}

void link_grid_room_tiles(maze_tile_t *head, maze_tile_t *current, maze_t *maze)
{
  maze_room_t *head_room_hit = get_intersecting_room(head, maze);
  maze_room_t *current_room_hit = get_intersecting_room(current, maze);
  grid_maze_tile_t *head_grid_tile = head->tile_data;
  grid_maze_tile_t *current_grid_tile = current->tile_data;
  maze_tile_t *head_adjacent;
  int distance = abs(head_grid_tile->i - current_grid_tile->i) + abs(head_grid_tile->j - current_grid_tile->j);
  if (distance > 1) {
    maze_tile_t temp;
    temp.type = GRID_TILE;
    grid_maze_tile_t mt_temp;
    mt_temp.i = current_grid_tile->i;
    mt_temp.j = current_grid_tile->j - 1;
    temp.tile_data = &mt_temp;
    maze_room_t *up = get_intersecting_room(&temp, maze);
    mt_temp.j = current_grid_tile->j + 1;
    maze_room_t *down = get_intersecting_room(&temp, maze);
    mt_temp.j = current_grid_tile->j;
    mt_temp.i = current_grid_tile->i - 1;
    maze_room_t *left = get_intersecting_room(&temp, maze);
    mt_temp.i = current_grid_tile->i + 1;
    maze_room_t *right = get_intersecting_room(&temp, maze);
    if (up != NULL) {
      mt_temp.i = current_grid_tile->i;
      mt_temp.j = current_grid_tile->j - 1;
    } else if (right != NULL) {
      mt_temp.i = current_grid_tile->i + 1;
      mt_temp.j = current_grid_tile->j;
    } else if (down != NULL) {
      mt_temp.i = current_grid_tile->i;
      mt_temp.j = current_grid_tile->j + 1;
    } else if (left != NULL) {
      mt_temp.i = current_grid_tile->i - 1;
      mt_temp.j = current_grid_tile->j;
    } else {
      printf("### BOOP ###\n");
    }
    //printf("t0 %d, %d\n", mt_temp.i, mt_temp.j);
    //printf("t1 %d, %d\n", current_grid_tile->i, current_grid_tile->j);
    grid_maze_data_t *grid_maze = maze->maze_data;
    //maze_tile_t **tiles = grid_maze->tiles;
    //printf("mt_temp: %d, %d\n", mt_temp.j, mt_temp.i);
    //printf("mt_temp: %d %d %d %d\n", up == NULL, right == NULL, down == NULL, left == NULL);
    head_adjacent = &(grid_maze->tiles[mt_temp.j][mt_temp.i]);
    link_grid_tiles(head_adjacent, current, maze);
    ((grid_maze_tile_t*)head_adjacent->tile_data)->type = no_passages;
  } else {
    link_grid_tiles(head, current, maze);
  }

  if (head_room_hit != NULL && distance == 1) {
    //printf("Path out of room. Distance: %d\n", distance);
    //print_maze_room(head_room_hit);
    //printf("head ");
    //print_maze_tile(head);
    head_grid_tile->type = no_passages;
  } else if (distance > 1) {
    //printf("Path out of room. Distance: %d\n", distance);
    //print_maze_room(head_room_hit);
    //printf("head ");
    //print_maze_tile(head);
    //printf("head_adjacent ");
    //print_maze_tile(head_adjacent);
  }

  if (current_room_hit != NULL) {
    //printf("Path into room. Distance: %d\n", distance);
    //print_maze_room(current_room_hit);
    //printf("head ");
    //print_maze_tile(head);
    current_grid_tile->type = no_passages;
  }

  if (current_room_hit != NULL || head_room_hit != NULL || distance > 1) {
    //printf("current ");
    //print_maze_tile(current);
    //printf("\n");
  }
}

maze_room_t *get_intersecting_room(maze_tile_t *tile, maze_t *maze)
{
  grid_maze_room_data_t *room_maze = maze->maze_data;
  grid_maze_tile_t *grid_tile = tile->tile_data;
  maze_room_t *room_hit = NULL;
  for (int r = 0; r < room_maze->rooms_length; r++) {
    maze_room_t *room = room_maze->rooms[r];
    if (((grid_tile->i) >= (room->start_i) && (grid_tile->i) < (room->start_i + room->width)) &&
        ((grid_tile->j) >= (room->start_j) && (grid_tile->j) < (room->start_j + room->height))) {
      room_hit = room;
    }
  }
  return room_hit;
}

int get_room_perimiter_tiles(maze_room_t *room, maze_t *maze, maze_tile_t *permiter_tiles[])
{
  grid_maze_data_t *grid_maze = maze->maze_data;
  maze_tile_t **tiles = grid_maze->tiles;
  int permiter_tiles_length = 0;

  if ((room->start_j - 1) >= 0) {
    for (int i = 0; i < room->width; i++) {
      permiter_tiles[permiter_tiles_length] = &tiles[room->start_j - 1][room->start_i + i];
      permiter_tiles_length++;
    }
  }

  if ((room->start_j + room->height) < grid_maze->height) {
    for (int k = 0; k < room->width; k++) {
      permiter_tiles[permiter_tiles_length] = &tiles[room->start_j + room->height][room->start_i + k];
      permiter_tiles_length++;
    }
  }

  if ((room->start_i - 1) >= 0) {
    for (int j = 0; j < room->height; j++) {
      permiter_tiles[permiter_tiles_length] = &tiles[room->start_j + j][room->start_i - 1];
      permiter_tiles_length++;
    }
  }

  if ((room->start_i + room->width) < grid_maze->width) {
    for (int l = 0; l < room->height; l++) {
      permiter_tiles[permiter_tiles_length] = &tiles[room->start_j + l][room->start_i + room->width];
      permiter_tiles_length++;
    }
  }

  return permiter_tiles_length;
}

void print_maze_room(maze_room_t *room)
{
  printf("room: %d, %d, %d, %d\n", room->start_i, room->start_j, room->width, room->height);
}

void print_maze_tile(maze_tile_t *tile)
{
  grid_maze_tile_t *g_tile = tile->tile_data;
  printf("tile: %d, %d\n", g_tile->i, g_tile->j);
}
