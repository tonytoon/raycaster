#include "sector.h"
#include "vectors.h"
#include <stdint.h>

void render_sector(uint32_t *buffer, int column_start, int column_end,
                   int view_h, Vec2 pos, Vec2 dir, float fov, int sector_idx);
int point_in_sector(Vec2 pos, int last_known_sector);

/*void collide_player_with_sector(World *world, int sector_idx, Vec2 *pos,
                                float radius);*/

void collide_and_traverse(int *sector_idx, Vec2 *pos, Vec2 prev_pos,
                          float radius);
