#include <stddef.h>
#include <stdio.h>
#include "grid_maze_topology.h"
#include "grid_maze_rooms_topology.h"


maze_room_t *get_intersecting_room(maze_tile_t *tile, maze_t *maze);
int get_room_perimiter_tiles(maze_room_t *room, maze_t *maze, maze_tile_t *permiter_tiles[]);

int get_grid_room_neighbors(maze_tile_t *tile, maze_tile_t *neighbors[], maze_t *maze)
{
  grid_maze_room_data_t *room_maze = maze->maze_data;
  grid_maze_data_t *grid_maze = maze->maze_data;
  maze_tile_t *all_neighbors[MAX_NEIGHBORS];
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
    for (int p = 0; p < perimeter_tiles_length; p++) {
      non_intersecting_neighbors[non_intersecting_neighbors_length] = perimeter_tiles[p];
      non_intersecting_neighbors_length++;
    }
  }

  int neighbors_length = get_valid_grid_neighbors(non_intersecting_neighbors_length, non_intersecting_neighbors, neighbors);

  return neighbors_length;
}

void link_grid_room_tiles(maze_tile_t *head, maze_tile_t *current, maze_t *maze)
{

  link_grid_tiles(head, current, maze);

  maze_room_t *head_room_hit = get_intersecting_room(head, maze);
  maze_room_t *current_room_hit = get_intersecting_room(current, maze);

  if (head_room_hit != NULL) {
    grid_maze_tile_t *head_grid_tile = head->tile_data;
    head_grid_tile->type = no_passages;
  }
  if (current_room_hit != NULL) {
    grid_maze_tile_t *current_grid_tile = current->tile_data;
    current_grid_tile->type = no_passages;
  }
}

maze_room_t *get_intersecting_room(maze_tile_t *tile, maze_t *maze)
{
  grid_maze_room_data_t *room_maze = maze->maze_data;
  grid_maze_tile_t *next_head = tile->tile_data;
  maze_room_t *room_hit = NULL;
  for (int r = 0; r < room_maze->rooms_length; r++) {
    maze_room_t *room = room_maze->rooms[r];
    if (((next_head->i) >= (room->start_i) && (next_head->i) <= (room->start_i + room->width)) &&
        ((next_head->j) >= (room->start_j) && (next_head->j) <= (room->start_j + room->height))) {
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
  for (int i = room->start_i; i < room->width - 1; i++) {
    permiter_tiles[permiter_tiles_length] = &tiles[room->start_j][i];
    permiter_tiles_length++;
  }

  for (int k = room->start_i + 1; k < room->width; k++) {
    permiter_tiles[permiter_tiles_length] = &tiles[room->height][k];
    permiter_tiles_length++;
  }

  for (int j = room->start_j + 1; j < room->height; j++) {
    permiter_tiles[permiter_tiles_length] = &tiles[j][room->start_i];
    permiter_tiles_length++;
  }

  for (int l = room->start_j; l < room->height - 1; l++) {
    permiter_tiles[permiter_tiles_length] = &tiles[l][room->width];
    permiter_tiles_length++;
  }

  return permiter_tiles_length;
}
