#ifndef grid_maze
#define grid_maze

typedef struct {
  int i;
  int j;
} maze_tile;

typedef enum {
  no_passages = 0,
  passage_up = 1,
  passage_right = 2,
  passage_down = 4,
  passage_left = 8,
} tile_type;

typedef struct {
  tile_type type;
} tile;

typedef struct {
  int height;
  int width;
  tile **tiles;
} maze;

void generate_maze(maze *maze);

#endif
