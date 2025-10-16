#include "sdlstuff.h"
#include <SDL.h>

extern uint32_t *view_framebuffer;
extern uint32_t *map_framebuffer;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *view;
extern SDL_Texture *maptex;

int initsdl() {
  window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, VIEWWIDTH, VIEWHEIGHT, 0);
  if (!window) {
    SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
    return 1;
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
    return 1;
  }
  SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
      view_framebuffer, VIEWWIDTH, VIEWHEIGHT, 32, VIEWWIDTH * sizeof(uint32_t),
      R_MASK, G_MASK, B_MASK, A_MASK);
  view = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  return 0;
}

void quitsdl() { SDL_Quit(); }
