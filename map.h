#ifndef MAP_H
#define MAP_H

#define MAPWIDTH 24
#define MAPHEIGHT 24

#include <SDL.h>
#include <stdlib.h>

#define N_WALL 8
#define E_WALL 4
#define S_WALL 2
#define W_WALL 1

typedef struct {
  uint8_t walls;
  uint32_t n_color;
  uint32_t e_color;
  uint32_t s_color;
  uint32_t w_color;
} Tile;

typedef struct {
  int w;
  int h;
  Tile *tiles;
} Map;

Map *loadmap(char *filename);
Map *createmap();

SDL_Texture *render_map(Map *map, double px, double py, double dx, double dy);

#endif
