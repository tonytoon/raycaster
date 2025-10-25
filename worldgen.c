#include "worldgen.h"
#include "math.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Utility for shuffling directions
void shuffle(int *array, int n) {
  for (int i = n - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
  }
}

Vec2 sector_centroid(Sector *s, Wall *walls) {
  Vec2 centroid = {0, 0};
  for (int i = 0; i < s->wall_count; i++) {
    Vec2 a = walls[s->wall_start + i].a;
    centroid.x += a.x;
    centroid.y += a.y;
  }
  centroid.x /= s->wall_count;
  centroid.y /= s->wall_count;
  return centroid;
}

float angle_from_centroid(Vec2 centroid, Vec2 point) {
  return atan2(point.y - centroid.y, point.x - centroid.x);
}
void fix_sector_orientation(Sector *s, Wall *walls) {
  Vec2 centroid = sector_centroid(s, walls);

  // Simple bubble sort (or qsort) by angle from centroid
  for (int i = 0; i < s->wall_count - 1; i++) {
    for (int j = i + 1; j < s->wall_count; j++) {
      Vec2 a_i = walls[s->wall_start + i].a;
      Vec2 a_j = walls[s->wall_start + j].a;

      float angle_i = angle_from_centroid(centroid, a_i);
      float angle_j = angle_from_centroid(centroid, a_j);

      if (angle_i > angle_j) {
        Wall tmp = walls[s->wall_start + i];
        walls[s->wall_start + i] = walls[s->wall_start + j];
        walls[s->wall_start + j] = tmp;
      }
    }
  }
}

// Recursive DFS to carve paths
void carve_maze(int *visited, int width, int height, int x, int y,
                World *world) {
  int dirs[4] = {0, 1, 2, 3}; // up, right, down, left
  shuffle(dirs, 4);
  int cell = y * width + x;
  visited[cell] = 1;

  for (int i = 0; i < 4; i++) {
    int nx = x, ny = y;
    switch (dirs[i]) {
    case 0:
      ny = y - 1;
      break; // up
    case 1:
      nx = x + 1;
      break; // right
    case 2:
      ny = y + 1;
      break; // down
    case 3:
      nx = x - 1;
      break; // left
    }

    if (nx < 0 || nx >= width || ny < 0 || ny >= height)
      continue;

    int neighbor = ny * width + nx;
    if (visited[neighbor])
      continue;

    // Remove wall between cell and neighbor
    int wall_idx = world->sectors[cell].wall_start + dirs[i];
    world->walls[wall_idx].neighbor = neighbor;

    // Also remove neighbor's corresponding wall
    int opposite = (dirs[i] + 2) % 4;
    int neighbor_wall_idx = world->sectors[neighbor].wall_start + opposite;
    world->walls[neighbor_wall_idx].neighbor = cell;

    carve_maze(visited, width, height, nx, ny, world);
  }
}

World *generate_maze(int width, int height, float cell_size) {
  int num_cells = width * height;
  int max_walls_per_sector = 4;

  World *world = malloc(sizeof(World));
  world->num_sectors = num_cells;
  world->num_walls = num_cells * max_walls_per_sector;
  world->sectors = malloc(sizeof(Sector) * num_cells);
  world->walls = malloc(sizeof(Wall) * world->num_walls);

  int wall_idx = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int cell = y * width + x;
      world->sectors[cell].wall_start = wall_idx;
      world->sectors[cell].wall_count = 4;
      world->sectors[cell].floor_height = rand() % 9; // 0;
      world->sectors[cell].ceiling_height = 32 - (rand() % 9);

      float cx = x * cell_size;
      float cy = y * cell_size;

      // Top wall
      world->walls[wall_idx++] =
          (Wall){{cx, cy},   {cx + cell_size, cy}, 0xff505050,
                 0xff808080, 0xff505050,           -1};
      // Right wall
      world->walls[wall_idx++] =
          (Wall){{cx + cell_size, cy}, {cx + cell_size, cy + cell_size},
                 0xff505050,           0xff808080,
                 0xff505050,           -1};
      // Bottom wall
      world->walls[wall_idx++] = (Wall){{cx + cell_size, cy + cell_size},
                                        {cx, cy + cell_size},
                                        0xff505050,
                                        0xff808080,
                                        0xff505050,
                                        -1};
      // Left wall
      world->walls[wall_idx++] =
          (Wall){{cx, cy + cell_size}, {cx, cy},   0xff505050,
                 0xff808080,           0xff505050, -1};
    }
  }

  int *visited = calloc(num_cells, sizeof(int));
  srand(time(NULL));
  carve_maze(visited, width, height, 0, 0, world);
  free(visited);

  return world;
}
