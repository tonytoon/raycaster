#include "input.h"
#include "map.h"
#include "player.h"
#include "raycaster.h"
#include "sdlstuff.h"
#include <SDL.h>
#include <SDL_render.h>
#include <stdint.h>
#include <stdio.h>

// globals we'll need
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *view = NULL;
SDL_Texture *maptex = NULL;
uint32_t view_framebuffer[VIEWWIDTH * VIEWHEIGHT];
uint32_t map_framebuffer[VIEWHEIGHT * VIEWHEIGHT];

Player player = {.pos = {22, 12},
                 .dir = {-1, 0},
                 .viewplane = {0, -0.66},
                 .speed = 1.0,
                 .rot = 1.0};

void gameloop() {
  int running = 1;
  SDL_Event e;
  Input input = {0};
  uint32_t time = 0;
  uint32_t prevtime = 0;
  Map *map = createmap();
  SDL_Rect viewarea = {0, 0, VIEWWIDTH, VIEWHEIGHT};
  SDL_Rect maparea = {VIEWWIDTH, 0, VIEWHEIGHT, VIEWHEIGHT};
  while (running) {
    // Handle events
    prevtime = time;
    time = SDL_GetTicks();
    double frame_time = (time - prevtime) / 1000.00;
    player.speed = frame_time * 5.0;
    player.rot = frame_time * 3.0;
    printf("FPS: %f\n", 1.0 / frame_time);

    getinput(&input, &running);
    handleinput(&input, &player, map);
    render_untextured(view_framebuffer, map, VIEWWIDTH, VIEWHEIGHT, player.pos,
                      player.dir, player.viewplane);
    maptex =
        render_map(map, player.pos.x, player.pos.y, player.dir.x, player.dir.y);
    SDL_UpdateTexture(view, NULL, view_framebuffer,
                      VIEWWIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, view, NULL, &viewarea);
    SDL_RenderCopy(renderer, maptex, NULL, &maparea);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(maptex);
  }
}

int main() {
  initsdl();
  gameloop();
  quitsdl();
  return 0;
}
