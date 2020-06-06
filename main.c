#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

void render_tiles(int height, int width, tile tiles[height][width]);
void render_tiles_small(int height, int width, tile tiles[height][width]);

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

  tile tiles[height][width];
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      tile t;
      t.type = no_passages;
      t.flag = 0;
      tiles[j][i] = t;
    }
  }

  generate_maze(height, width, tiles);

  //render_tiles_small(height, width, tiles);
  render_tiles(height, width, tiles);

  return 0;
}



void render_tiles(int height, int width, tile tiles[height][width])
{
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
        if (t_j_u < t_j && t_j_u > 0 && (tiles[t_j][t_i].type & passage_up) == passage_up) {
          printf("i");
        } else if (t_j_u < t_j && t_j > 0 && (tiles[t_j - 1][t_i].type & passage_down) == passage_down) {
          printf("j");
        } else if (t_j < t_j_d && t_j_d < height && (tiles[t_j][t_i].type & passage_down) == passage_down) {
          printf(".");
        } else if (t_j < t_j_d && t_j + 1 < height && (tiles[t_j + 1][t_i].type & passage_up) == passage_up) {
          printf(".");
        } else {
          printf("#");
        }
      } else if (j % 2 == 0) {
        if (t_i_l < t_i && t_i_l > 0 && (tiles[t_j][t_i].type & passage_left) == passage_left) {
          printf("a");
        } else if (t_i_l < t_i && t_i > 0 && (tiles[t_j][t_i - 1].type & passage_right) == passage_right) {
          printf("b");
        } else if (t_i < t_i_r && t_i_r < width && (tiles[t_j][t_i].type & passage_right) == passage_right) {
          printf(".");
        } else if (t_i < t_i_r && t_i + 1 < width && (tiles[t_j][t_i + 1].type & passage_left) == passage_left) {
          printf(".");
        } else {
          printf("#");
        }
      } else {
        printf(".");
      }
    }
    printf("\n");
  }
}

void render_tiles_small(int height, int width, tile tiles[height][width])
{
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      char *c;
      switch (tiles[j][i].type) {
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
        sprintf(str, "%d", tiles[j][i].type); break;
        c = str;
      }
      }
      printf("%s", c);
    }
    printf("\n");
  }
}
