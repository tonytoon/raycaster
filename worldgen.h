#include "sector.h"

typedef struct {
  int x, y;
  int visited;
  int walls[4]; // top, right, bottom, left (1=wall exists)
} Cell;

World *generate_maze(int width, int height, float cell_size);

// World *create_maze_sector(int grid_w, int grid_h, float cell_size);
