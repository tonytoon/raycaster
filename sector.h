#ifndef SECTOR_H
#define SECTOR_H

#include "vectors.h"
#include <stdlib.h>

typedef struct Sector {
  float floor_height;
  float ceiling_height;
  int wall_start;
  int wall_count;
} Sector;

typedef struct Wall {
  Vec2 a, b;
  uint32_t rgb_color;
  int neighbor;
  int texture_index;
} Wall;

typedef struct World {
  Sector *sectors;
  Wall *walls;
  Vec2 *vertices;
  int num_sectors;
  int num_walls;
  int num_vertices;
} World;

typedef struct Ray {
  Vec2 origin;
  Vec2 dir;
} Ray;

World *createworld();
World *create_two_sector_world();
World *create_demo_world();
World *create_four_connected_sectors();
World *create_two_small_sectors();

#endif
