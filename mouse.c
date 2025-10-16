#include "map.h"

void mouseview(int *x, int *y);

int mouseonmap(Map *map) {

  int mousex, mousey;
  SDL_GetMouseState(&mousex, &mousey);
  int mapmousex = (mousex - 640) / (480 / map->w);
  int mapmousey = (mousey / (480 / map->h));
  if (mapmousex >= 0 && mapmousex <= 23 && mapmousey >= 0 && mapmousey <= 23) {
    return 1;
  }
  return 0;
}

void mousemap(Map *map, int *x, int *y) {
  int tx, ty;
  SDL_GetMouseState(&tx, &ty);
  *x = (tx - 640) / (480 / map->w);
  *y = (ty / (480 / map->h));
}
