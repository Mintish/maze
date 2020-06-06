#ifndef maze
#define maze

typedef struct {
  int i;
  int j;
} maze_tile;

typedef enum {
  explored = 1
} flag;

typedef enum {
  no_passages = 0,
  passage_up = 1,
  passage_right = 2,
  passage_down = 4,
  passage_left = 8,
} tile_type;

typedef struct {
  tile_type type;
  flag flag;
} tile;

void generate_maze(int height, int width, tile tiles[height][width]);

#endif
