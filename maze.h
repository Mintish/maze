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

maze_tile get_random_grid_perimeter_tile(maze *maze);

int get_grid_neighbors(maze_tile tile, maze_tile neighbors[], maze* maze);

void link_grid_tiles(maze_tile head, maze_tile current, maze *maze);

void generate_maze(maze *maze_data, maze_tile(*get_start_tile)(maze*), int(*get_neighbors)(maze_tile, maze_tile[], maze*), void(*link_tiles)(maze_tile, maze_tile, maze*));

#endif
