#include <stdlib.h>
#include "grid_maze_rooms_topology.h"

int overlaps(int start_i, int width_i, int start_j, int width_j);

int generate_rooms(int maze_height, int maze_width, int rooms_length, maze_room_t **rooms)
{
  int room_count = 0;

  int tries_remaining = rooms_length * 10000;

  while (room_count < rooms_length && tries_remaining > 0) {
    int start_i = rand() % maze_width;
    int start_j = rand() % maze_height;
    int width = rand() % 20;
    int height = rand() % 20;

    int overlaps_count = 0;

    if (start_i <= 0 || start_j <= 0 || (start_i + width) >= maze_width || (start_j + height) >= maze_height) {
      overlaps_count++;
    }

    for (int r = 0; r < room_count; r++) {
      int overlaps_i = overlaps(start_i - 1, width + 1, rooms[r]->start_i - 1, rooms[r]->width + 1);
      int overlaps_j = overlaps(start_j - 1, height + 1, rooms[r]->start_j - 1, rooms[r]->height + 1);

      if (overlaps_i && overlaps_j) {
        overlaps_count++;
      }
    }

    if (overlaps_count == 0) {
      maze_room_t *current = malloc(sizeof(maze_room_t));
      current->start_i = start_i;
      current->start_j = start_j;
      current->width = width;
      current->height = height;
      current->marked_by = NULL;
      current->flags = unvisited;
      rooms[room_count] = current;
      room_count++;

      /*
      for (int i = 0; i < width && start_x + i < WIDTH; i++) {
        for (int j = 0; j < height && start_y + j < HEIGHT; j++) {
          tiles[start_y  + j][start_x + i] = 1;
        }
      }
      */
    }
    tries_remaining--;
  }

  return room_count;
}

int overlaps(int start_i, int width_i, int start_j, int width_j)
{
  int start;
  if (start_i < start_j) {
    start = start_i;
  } else {
    start = start_j;
  }

  int end_i = start_i + width_i;
  int end_j = start_j + width_j;
  int end;
  if (end_i > end_j) {
    end = end_i;
  } else {
    end = end_j;
  }

  int width_proj = end - start;
  int total_width = width_i + width_j;

  return width_proj < total_width;
}
