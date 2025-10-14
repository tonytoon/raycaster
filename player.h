#ifndef PLAYER_H
#define PLAYER_H

#include "input.h"
#include "map.h"
#include "vectors.h"

typedef struct {
  Vec2 pos;       // player position
  Vec2 dir;       // direction facing
  Vec2 viewplane; // camera plane (for raycasting)
  double speed;   // move speed per frame
  double rot;     // rotation speed per frame
} Player;

void handleinput(Input *input, Player *player, Map *map);

#endif
