// debug.h
#pragma once
#include "player.h"
#include "sector.h"
#include <SDL2/SDL.h>

void debug_draw_world(SDL_Renderer *renderer, World *world, int scale);
void debug_draw_player(SDL_Renderer *renderer, Player *player);
void debug_draw_portals(SDL_Renderer *renderer, World *world);
