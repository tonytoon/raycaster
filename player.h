#ifndef PLAYER_H
#define PLAYER_H
#include "collision.h"
#include "input.h"
#include "raycaster.h"
#include "sector.h"
#include "vectors.h"

extern World world;

typedef struct {
  Vec2 pos;       // player position
  Vec2 dir;       // direction facing
  Vec2 viewplane; // camera plane (for raycasting)
  float speed;    // move speed per frame
  float rot;      // rotation speed per frame
  float radius;
  int current_sector;
} Player;

void handleinput(Input *input, Player *player);

#endif
