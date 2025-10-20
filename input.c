#include "input.h"
#include <SDL.h>

void getinput(Input *in, int *running) {
  // pressed keys
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT)
      *running = 0;

    else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
      int down = (e.type == SDL_KEYDOWN);

      switch (e.key.keysym.sym) {
      case SDLK_w:
        in->move_forward = down;
        break;
      case SDLK_s:
        in->move_backward = down;
        break;
      case SDLK_a:
        in->move_left = down;
        break;
      case SDLK_d:
        in->move_right = down;
        break;
      case SDLK_LEFT:
        in->turn_left = down;
        break;
      case SDLK_RIGHT:
        in->turn_right = down;
        break;
      case SDLK_SPACE:
        in->change_sector = down;
        break;
      case SDLK_ESCAPE:
        *running = 0;
        break;
      }
    }
  }
}
