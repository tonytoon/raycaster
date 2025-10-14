#include "map.h"
#include <stdio.h>

int worldMap[24][24] = {
    {9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 12},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 4, 9, 8, 8, 8, 12, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 4, 1, 0, 0, 0, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 4, 1, 0, 0, 0, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 4, 1, 0, 0, 0, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 4, 1, 0, 0, 0, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 4, 3, 2, 0, 2, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 8, 8, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6}};

Map *loadmap(char *filename) {
  FILE *file = fopen(filename, "r");
  return NULL;
}

Map *createmap() {
  Map *map = malloc(sizeof(Map));
  map->w = 24;
  map->h = 24;
  map->tiles = malloc(map->w * map->h * sizeof(Tile));
  for (int x = 0; x != map->w; x++) {
    for (int y = 0; y != map->h; y++) {
      map->tiles[y * map->w + x].walls = worldMap[y][x];
    }
  }
  for (int y = 0; y != map->h; y++) {
    for (int x = 0; x != map->w; x++) {
      printf("%d", map->tiles[y * map->w + x].walls);
    }
    printf("\n");
  }

  return map;
}

#define VIEWHEIGHT 480
SDL_Texture *render_map(Map *map, double px, double py, double dx, double dy) {
  // ugly hack to make buffer 2d for my sanity
  extern SDL_Renderer *renderer;
  SDL_Texture *map_texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                        SDL_TEXTUREACCESS_TARGET, map->w * 10, map->h * 10);
  SDL_SetTextureBlendMode(map_texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, map_texture);

  for (int y = 0; y != map->h; y++)
    for (int x = 0; x != map->w; x++) {
      SDL_Rect rect = {x * 10, y * 10, 10, 10};

      SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);
      SDL_RenderDrawRect(renderer, &rect);
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      if (map->tiles[y * map->h + x].walls & N_WALL) {
        SDL_RenderDrawLine(renderer, x * 10, y * 10, x * 10 + 10, y * 10);
      }
      if (map->tiles[y * map->h + x].walls & E_WALL) {
        SDL_RenderDrawLine(renderer, x * 10 + 9, y * 10, x * 10 + 9,
                           y * 10 + 10);
      }
      if (map->tiles[y * map->h + x].walls & S_WALL) {
        SDL_RenderDrawLine(renderer, x * 10, y * 10 + 9, x * 10 + 10,
                           y * 10 + 9);
      }
      if (map->tiles[y * map->h + x].walls & W_WALL) {
        SDL_RenderDrawLine(renderer, x * 10, y * 10, x * 10, y * 10 + 10);
      }
    }
  SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
  int npx = (int)(px * 10);
  int npy = (int)(py * 10);
  SDL_Rect rect = {npx - 3, npy - 3, 6, 6};
  SDL_RenderFillRect(renderer, &rect);
  SDL_RenderDrawLine(renderer, npx, npy, npx + dx * 40, npy + dy * 40);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_SetRenderTarget(renderer, NULL);
  return map_texture;
}
