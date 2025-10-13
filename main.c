#include "input.h"
#include "map.h"
#include "player.h"
#include "raycaster.h"
#include "sdlstuff.h"
#include <SDL.h>
#include <SDL_render.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

// globals we'll need
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

uint32_t framebuffer[VIEWWIDTH * VIEWHEIGHT];

Player player = {.pos = {22, 12},
                 .dir = {-1, 0},
                 .viewplane = {0, 2.0 / 3.0},
                 .speed = 1,
                 .rot = 1};

void gameloop() {
  int running = 1;
  SDL_Event e;
  double turn_angle = M_PI / 180;
  Input input = {0};
  uint32_t time = 0;
  uint32_t prevtime = 0;

  while (running) {
    // Handle events
    prevtime = time;
    time = SDL_GetTicks();
    double frame_time = (time - prevtime) / 1000.00;
    player.speed = frame_time * 5.0;
    player.rot = frame_time * 3.0;

    getinput(&input, &running);
    handleinput(&input, &player);
    render_untextured(framebuffer, &worldMap[0][0], MAPWIDTH, MAPHEIGHT,
                      VIEWWIDTH, VIEWHEIGHT, player.pos, player.dir,
                      player.viewplane);

    SDL_UpdateTexture(texture, NULL, framebuffer, VIEWWIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }
}

int main() {
  initsdl();
  gameloop();
  quitsdl();
  return 0;
}
