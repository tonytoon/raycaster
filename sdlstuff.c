#include "sdlstuff.h"
#include <SDL.h>

extern uint32_t *framebuffer;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;

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
  // TODO fix for big/little endian
  SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
      framebuffer, VIEWWIDTH, VIEWHEIGHT, 32, VIEWWIDTH * sizeof(uint32_t),
      0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  return 0;
}

void quitsdl() { SDL_Quit(); }
