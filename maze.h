#ifndef MAZE
#define MAZE

#define MAX_NEIGHBORS 256

typedef struct {
  void *maze_data;
} maze_t;

typedef struct {
  void *tile_data;
} maze_tile_t;

#endif
