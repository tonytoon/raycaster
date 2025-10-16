#include "sector.h"

World *createworld() {
  World *world = malloc(sizeof(World));

  // one sector with four walls to start
  world->sectors = malloc(sizeof(Sector));
  world->sectors[0].floor_height = 0;
  world->sectors[0].ceiling_height = 64;
  world->sectors[0].wall_start = 0;
  world->sectors[0].wall_count = 7;

  world->walls = malloc(sizeof(Wall) * 255);
  world->walls[0].a = (Vec2){0, 0};
  world->walls[0].b = (Vec2){64, 0};
  world->walls[0].rgb_color = 0xffff0000;
  world->walls[1].a = (Vec2){64, 0};
  world->walls[1].b = (Vec2){64, 64};
  world->walls[1].rgb_color = 0xff00ff00;
  world->walls[2].a = (Vec2){64, 64};
  world->walls[2].b = (Vec2){0, 64};
  world->walls[2].rgb_color = 0xff0000ff;
  world->walls[3].a = (Vec2){0, 64};
  world->walls[3].b = (Vec2){0, 0};
  world->walls[3].rgb_color = 0xffffffff;

  world->walls[4].a = (Vec2){24, 24};
  world->walls[4].b = (Vec2){36, 48};
  world->walls[4].rgb_color = 0xffffff00;

  world->walls[5].a = (Vec2){36, 48};
  world->walls[5].b = (Vec2){48, 24};
  world->walls[5].rgb_color = 0xffff00ff;

  world->walls[6].a = (Vec2){48, 24};
  world->walls[6].b = (Vec2){24, 24};
  world->walls[6].rgb_color = 0xff00ffff;

  return world;
}
