#include<stdio.h>

/*
 * 0 - no passages
 * 1 - passage up 
 * 2 - passage right 
 * 3 - passage down
 * 4 - passage left
 * 5 - passage up and right
 * 6 - passage up and down
 * 7 - passage up and left
 * 8 - passage right and down
 * 9 - passage right and left
 * 10- passage down and left
 */

typedef enum {
  no_passages = 0,
  passage_up = 1,
  passage_right = 2,
  passage_down = 4,
  passage_left = 8,
} tile_type;

void render_tiles(int height, int width, tile_type tiles[3][3])
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
        if (t_j_u < t_j && (tiles[t_j][t_i] & passage_up) == passage_up) {
          printf(".");
        } else if (t_j_u < t_j && t_j > 0 && (tiles[t_j - 1][t_i] & passage_down) == passage_down) {
          printf(".");
        } else if (t_j < t_j_d && (tiles[t_j][t_i] & passage_down) == passage_down) {
          printf(".");
        } else if (t_j < t_j_d && t_j + 1 < height && (tiles[t_j + 1][t_i] & passage_up) == passage_up) {
          printf(".");
        } else {
          printf("#", t_j);
        }
      } else if (j % 2 == 0) {
        if (t_i_l < t_i && (tiles[t_j][t_i] & passage_left) == passage_left) {
          printf(".");
        } else if (t_i_l < t_i && t_i > 0 && (tiles[t_j][t_i - 1] & passage_right) == passage_right) {
          printf(".");
        } else if (t_i < t_i_r && (tiles[t_j][t_i] & passage_right) == passage_right) {
          printf(".");
        } else if (t_i < t_i_r && t_i + 1 < width && (tiles[t_j][t_i + 1] & passage_left) == passage_left) {
          printf(".");
        } else {
          printf("#");
        }
      } else {
        printf(".", t_j);
      }
    }
    printf("\n");
  }
}

int main() {

  tile_type tiles[3][3];
  /*
  tiles[0][0] = no_passages;
  tiles[0][1] = no_passages;
  tiles[0][2] = no_passages;

  tiles[1][0] = no_passages;
  tiles[1][1] = passage_right | passage_left | passage_up | passage_down;
  tiles[1][2] = no_passages;

  tiles[2][0] = no_passages;
  tiles[2][1] = no_passages;
  tiles[2][2] = no_passages;
  */

  tiles[0][0] = passage_right;
  tiles[0][1] = passage_right | passage_left;
  tiles[0][2] = passage_left | passage_down;

  tiles[1][0] = passage_down | passage_right;
  tiles[1][1] = passage_right | passage_left;
  tiles[1][2] = passage_left | passage_up;

  tiles[2][0] = passage_up | passage_right;
  tiles[2][1] = passage_left | passage_right;
  tiles[2][2] = passage_left;

  render_tiles(3, 3, tiles);

  return 0;
}
