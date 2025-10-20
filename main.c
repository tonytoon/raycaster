#include "debug.h"
#include "input.h"
#include "player.h"
#include "raycaster.h"
#include "sdlstuff.h"
#include "sector.h"
#include "worldgen.h"
#include <SDL.h>
#include <SDL_render.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// globals we'll need
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *view = NULL;
uint32_t view_framebuffer[VIEWWIDTH * VIEWHEIGHT];
uint32_t map_framebuffer[VIEWHEIGHT * VIEWHEIGHT];

Player player = {.pos = {5, 5},
                 .dir = {-1, 0},
                 .viewplane = {0, -0.66},
                 .speed = 1.0,
                 .rot = 1.0,
                 .current_sector = 0,
                 .radius = 1.0f};

World world;
void gameloop() {
  srand(time(NULL)); // Seed RNG with current time

  int running = 1;
  SDL_Event e;
  Input input = {0};
  uint32_t time = 0;
  uint32_t prevtime = 0;
  world = *generate_maze(20, 20, 3.0f);
  SDL_Rect viewarea = {0, 0, VIEWWIDTH, VIEWHEIGHT};
  float fov = 90.00 * M_PI / 180;
  while (running) {
    // Handle events
    prevtime = time;
    time = SDL_GetTicks();
    float frame_time = (time - prevtime) / 1000.00;
    player.speed = frame_time * 3.0;
    player.rot = frame_time * 3.0;
    getinput(&input, &running);
    handleinput(&input, &player);

    memset(view_framebuffer, 0, VIEWWIDTH * VIEWHEIGHT * sizeof(uint32_t));
    render_sector(view_framebuffer, 0, VIEWWIDTH, VIEWHEIGHT, player.pos,
                  player.dir, fov, player.current_sector);
    SDL_UpdateTexture(view, NULL, view_framebuffer,
                      VIEWWIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, view, NULL, &viewarea);
    // debug_draw_player(renderer, &player);
    // debug_draw_world(renderer, world, 4);
    SDL_RenderPresent(renderer);
  }
}

int main() {
  initsdl();
  gameloop();
  quitsdl();
  return 0;
}
