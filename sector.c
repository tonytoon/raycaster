#include "sector.h"
World *create_four_connected_sectors() {
  World *world = malloc(sizeof(World));
  world->num_sectors = 4;
  world->num_walls = 16; // 4 walls per sector
  world->sectors = malloc(sizeof(Sector) * 4);
  world->walls = malloc(sizeof(Wall) * world->num_walls);

  // --- Sector 0 (top-left) ---
  world->sectors[0].wall_start = 0;
  world->sectors[0].wall_count = 4;
  world->sectors[0].floor_height = 0;
  world->sectors[0].ceiling_height = 32;

  // Coordinates: (0,0) → (32,0) → (32,32) → (0,32)
  world->walls[0] = (Wall){{0, 0}, {32, 0}, 0xff0000ff, -1}; // solid top
  world->walls[1] =
      (Wall){{32, 0}, {32, 16}, 0xff00ff00, 1}; // portal right to sector 1
  world->walls[2] = (Wall){{32, 16}, {0, 16}, 0xff0000ff, -1}; // solid bottom
  world->walls[3] = (Wall){{0, 16}, {0, 0}, 0xff808080, -1};   // solid left

  // --- Sector 1 (top-right) ---
  world->sectors[1].wall_start = 4;
  world->sectors[1].wall_count = 4;
  world->sectors[1].floor_height = 0;
  world->sectors[1].ceiling_height = 32;

  // Coordinates: (32,0) → (64,0) → (64,32) → (32,32)
  world->walls[4] = (Wall){{32, 0}, {64, 0}, 0xff00ffff, -1};
  world->walls[5] = (Wall){{64, 0}, {64, 32}, 0xffff00ff, -1};
  world->walls[6] =
      (Wall){{64, 32}, {32, 32}, 0xff00ffff, 3}; // portal down to sector 3
  world->walls[7] =
      (Wall){{32, 32}, {32, 0}, 0xffff00ff, 0}; // portal left to sector 0

  // --- Sector 2 (bottom-left) ---
  world->sectors[2].wall_start = 8;
  world->sectors[2].wall_count = 4;
  world->sectors[2].floor_height = 0;
  world->sectors[2].ceiling_height = 32;

  // Coordinates: (0,16) → (32,16) → (32,48) → (0,48)
  world->walls[8] =
      (Wall){{0, 16}, {32, 16}, 0xff00ff00, 0}; // portal up to sector 0
  world->walls[9] = (Wall){{32, 16}, {32, 48}, 0xffff0000, -1};
  world->walls[10] =
      (Wall){{32, 48}, {0, 48}, 0xff00ff00, 3}; // portal right to sector 3
  world->walls[11] = (Wall){{0, 48}, {0, 16}, 0xff808080, -1};

  // --- Sector 3 (bottom-right) ---
  world->sectors[3].wall_start = 12;
  world->sectors[3].wall_count = 4;
  world->sectors[3].floor_height = 0;
  world->sectors[3].ceiling_height = 32;

  // Coordinates: (32,32) → (64,32) → (64,48) → (32,48)
  world->walls[12] =
      (Wall){{32, 32}, {64, 32}, 0xffff00ff, 1}; // portal up to sector 1
  world->walls[13] = (Wall){{64, 32}, {64, 48}, 0xff00ffff, -1};
  world->walls[14] =
      (Wall){{64, 48}, {32, 48}, 0xffff00ff, 2}; // portal left to sector 2
  world->walls[15] = (Wall){{32, 48}, {32, 32}, 0xff00ffff, -1};

  return world;
}

World *create_demo_world() {
  World *world = malloc(sizeof(World));

  // Define number of sectors and walls
  world->num_sectors = 4;
  world->num_walls = 16; // 4 walls per sector
  world->sectors = malloc(sizeof(Sector) * world->num_sectors);
  world->walls = malloc(sizeof(Wall) * world->num_walls);

  // --- Sector 0: Top-left ---
  world->sectors[0] = (Sector){.wall_start = 0,
                               .wall_count = 4,
                               .floor_height = 0,
                               .ceiling_height = 32};
  world->walls[0] = (Wall){{0, 0}, {32, 0}, 0xff0000ff, -1};
  world->walls[1] =
      (Wall){{32, 0}, {32, 32}, 0xff00ff00, 1}; // portal to sector 1
  world->walls[2] = (Wall){{32, 32}, {0, 32}, 0xff0000ff, -1};
  world->walls[3] = (Wall){{0, 32}, {0, 0}, 0xff0000ff, -1};

  // --- Sector 1: Top-right ---
  world->sectors[1] = (Sector){.wall_start = 4,
                               .wall_count = 4,
                               .floor_height = 0,
                               .ceiling_height = 32};
  world->walls[4] = (Wall){{32, 0}, {64, 0}, 0xff00ffff, -1};
  world->walls[5] = (Wall){{64, 0}, {64, 32}, 0xffff00ff, -1};
  world->walls[6] = (Wall){{64, 32}, {32, 32}, 0xff00ffff, -1};
  world->walls[7] =
      (Wall){{32, 32}, {32, 0}, 0xffff00ff, 0}; // portal back to sector 0

  // --- Sector 2: Bottom-left ---
  world->sectors[2] = (Sector){.wall_start = 8,
                               .wall_count = 4,
                               .floor_height = 0,
                               .ceiling_height = 32};
  world->walls[8] = (Wall){{0, 32}, {32, 32}, 0xff808080, -1};
  world->walls[9] =
      (Wall){{32, 32}, {32, 64}, 0xff00ff00, 3}; // portal to sector 3
  world->walls[10] = (Wall){{32, 64}, {0, 64}, 0xff808080, -1};
  world->walls[11] = (Wall){{0, 64}, {0, 32}, 0xff808080, -1};

  // --- Sector 3: Bottom-right ---
  world->sectors[3] = (Sector){.wall_start = 12,
                               .wall_count = 4,
                               .floor_height = 0,
                               .ceiling_height = 32};
  world->walls[12] = (Wall){{32, 32}, {64, 32}, 0xff00ffff, -1};
  world->walls[13] = (Wall){{64, 32}, {64, 64}, 0xffff00ff, -1};
  world->walls[14] =
      (Wall){{64, 64}, {32, 64}, 0xff00ffff, 2}; // portal back to sector 2
  world->walls[15] = (Wall){{32, 64}, {32, 32}, 0xffff00ff, -1};

  return world;
}

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

#include <stdint.h>
#include <stdlib.h>

// Create two connected square sectors
World *create_two_sector_world() {
  World *world = malloc(sizeof(World));

  world->num_sectors = 2;
  world->num_walls = 8; // 4 walls per sector
  world->sectors = malloc(sizeof(Sector) * 2);
  world->walls = malloc(sizeof(Wall) * world->num_walls);

  // --- Sector 0 ---
  world->sectors[0].wall_start = 0;
  world->sectors[0].wall_count = 4;
  world->sectors[0].floor_height = 0;
  world->sectors[0].ceiling_height = 64;

  // Square from (0,0) to (128,128)
  world->walls[0] = (Wall){{0, 0}, {128, 0}, 0xff0000ff, -1}; // solid
  world->walls[1] =
      (Wall){{128, 0}, {128, 64}, 0xff00ff00, 0}; // portal to sector 1
  world->walls[2] = (Wall){{128, 64}, {0, 64}, 0xff0000ff, -1};
  world->walls[3] = (Wall){{0, 64}, {0, 0}, 0xff808080, -1};

  // --- Sector 1 ---
  world->sectors[1].wall_start = 4;
  world->sectors[1].wall_count = 4;
  world->sectors[1].floor_height = 0;
  world->sectors[1].ceiling_height = 64;

  // Square from (128,0) to (256,128)
  world->walls[4] = (Wall){{128, 0}, {256, 0}, 0xff00ffff, -1};
  world->walls[5] = (Wall){{256, 0}, {256, 128}, 0xffff00ff, -1};
  world->walls[6] = (Wall){{256, 128}, {128, 128}, 0xff00ffff, -1};
  world->walls[7] =
      (Wall){{128, 128}, {128, 64}, 0xffff00ff, 0}; // portal back to sector 0

  return world;
}

World *create_two_small_sectors() {
  World *world = malloc(sizeof(World));
  world->num_sectors = 2;
  world->num_walls = 8; // 4 walls per sector
  world->sectors = malloc(sizeof(Sector) * 2);
  world->walls = malloc(sizeof(Wall) * world->num_walls);

  // --- Sector 0 ---
  // Square from (0,0) to (32,16), CCW
  world->sectors[0].wall_start = 0;
  world->sectors[0].wall_count = 4;
  world->sectors[0].floor_height = 0;
  world->sectors[0].ceiling_height = 32;

  world->walls[0] = (Wall){{0, 0}, {32, 0}, 0xff0000ff, -1}; // solid
  world->walls[1] =
      (Wall){{32, 0}, {32, 16}, 0xff00ff00, 1}; // portal to sector 1
  world->walls[2] = (Wall){{32, 16}, {0, 16}, 0xff0000ff, -1}; // solid
  world->walls[3] = (Wall){{0, 16}, {0, 0}, 0xff808080, -1};   // solid

  // --- Sector 1 ---
  // Square from (32,0) to (64,16), CCW
  world->sectors[1].wall_start = 4;
  world->sectors[1].wall_count = 4;
  world->sectors[1].floor_height = 0;
  world->sectors[1].ceiling_height = 32;

  world->walls[4] = (Wall){{32, 0}, {64, 0}, 0xff00ffff, -1};   // solid
  world->walls[5] = (Wall){{64, 0}, {64, 16}, 0xffff00ff, -1};  // solid
  world->walls[6] = (Wall){{64, 16}, {32, 16}, 0xff00ffff, -1}; // solid
  world->walls[7] =
      (Wall){{32, 16}, {32, 0}, 0xffff00ff, 0}; // portal back to sector 0

  return world;
}

/*
World *create_two_small_sectors() {
  World *world = malloc(sizeof(World));

  world->num_sectors = 2;
  world->num_walls = 8; // 4 walls per sector
  world->sectors = malloc(sizeof(Sector) * 2);
  world->walls = malloc(sizeof(Wall) * world->num_walls);

  // --- Sector 0 ---
  world->sectors[0].wall_start = 0;
  world->sectors[0].wall_count = 4;
  world->sectors[0].floor_height = 0;
  world->sectors[0].ceiling_height = 32;

  // Square from (0,0) to (32,32)
  world->walls[0] = (Wall){{0, 0}, {32, 0}, 0xff0000ff, -1}; // solid
  world->walls[1] =
      (Wall){{32, 0}, {32, 16}, 0xff00ff00, 1}; // portal to sector 1
  world->walls[2] = (Wall){{32, 16}, {0, 16}, 0xff0000ff, -1};
  world->walls[3] = (Wall){{0, 16}, {0, 0}, 0xff808080, -1};

  // --- Sector 1 ---
  world->sectors[1].wall_start = 4;
  world->sectors[1].wall_count = 4;
  world->sectors[1].floor_height = 0;
  world->sectors[1].ceiling_height = 32;

  // Square from (32,0) to (64,32)
  world->walls[4] = (Wall){{32, 0}, {64, 0}, 0xff00ffff, -1};
  world->walls[5] = (Wall){{64, 0}, {64, 32}, 0xffff00ff, -1};
  world->walls[6] = (Wall){{64, 32}, {32, 32}, 0xff00ffff, -1};
  world->walls[7] =
      (Wall){{32, 32}, {32, 16}, 0xffff00ff, 0}; // portal back to sector 0

  return world;
} */
