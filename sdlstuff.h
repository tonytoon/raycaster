#ifndef SDLSTUFF_H
#define SDLSTUFF_H

#define VIEWWIDTH 640
#define VIEWHEIGHT 480
#include <SDL.h>
#include <stdlib.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define R_MASK 0xff000000
#define G_MASK 0x00ff0000
#define B_MASK 0x0000ff00
#define A_MASK 0x000000ff
#else
#define R_MASK 0x000000ff
#define G_MASK 0x0000ff00
#define B_MASK 0x00ff0000
#define A_MASK 0xff000000
#endif

int initsdl();
void quitsdl();

#endif
