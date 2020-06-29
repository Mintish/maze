#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfs_maze_generator.h"
#include "grid_maze_topology.h"

typedef struct {
  int height;
  int width;
  int *pixels;
} pgm_image;

void read_pgm_file(FILE *file, pgm_image *image);
void render_tile_line(pgm_image *image, int tiles_length, int tiles[tiles_length], int *line_max);

void render_tiles(int tiles_length, int stride, int tiles[tiles_length])
{
  FILE *mt = fopen("maze_tiles.pbm", "r");

  if (mt == NULL) {
    return;
  }

  pgm_image image;
  image.pixels = malloc(sizeof(int));
  read_pgm_file(mt, &image);
  fclose(mt);

  int tile_lines = tiles_length / stride;

  printf("P1\n");
  printf("%d %d\n", stride * image.width, tile_lines * image.width);
  int line_max = 0;
  for (int l = 0; l < tile_lines; l++) {
    render_tile_line(&image, stride, &tiles[l * stride], &line_max);
  }

  free(image.pixels);
}

void render_tile_line(pgm_image *image, int tiles_length, int tiles[tiles_length], int *line_max)
{
  int tile_height = image->width;
  int tile_width = image->width;
  for (int j = 0; j < tile_height; j++) {
    for (int i_i = 0; i_i < tile_width * tiles_length; i_i++) {
      int i = i_i % 10;
      int tile_offset = tiles[i_i / 10];
      printf("%d", image->pixels[i + ((j + (tile_offset * 10)) * tile_width)]);
      if (*line_max == 60) {
        printf("\n");
        *line_max = 0;
      } else {
        (*line_max)++;
      }
    }
  }
}

void read_pgm_file(FILE *file, pgm_image *image)
{
  char *line = NULL;
  size_t n = 0;
  ssize_t len;

  for (int l = 0; l < 3; l++, len = getline(&line, &n, file));

  char *width_s = strtok(line, " ");
  char *height_s = strtok(NULL, " ");
  int width = atoi(width_s);
  int height = atoi(height_s);

  free(line);

  int *pixels = image->pixels = realloc(image->pixels, sizeof(int) * height * width);

  char c;
  for (int i = 0; (c = fgetc(file)) != EOF; i++) {
    if (c != 10) {
      char c_s[10];
      sprintf(c_s, "%c", c);
      pixels[i] = atoi(c_s);
    } else {
      i--;
    }
  }

  image->height = height;
  image->width = width;
}
