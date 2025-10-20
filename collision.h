#include "sector.h"
#include "vectors.h"

extern World world;

Vec2 move_player(Vec2 pos, Vec2 prev_pos, float radius, int sector_idx);

int find_sector(Vec2 pos, int last_known_sector);
int point_in_sector(Vec2 pos, int last_known_sector);
