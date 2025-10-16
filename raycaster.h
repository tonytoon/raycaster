#include "sector.h"
#include "vectors.h"
#include <stdint.h>

void render_sector_untextured(uint32_t *buffer, World *world, int view_w,
                              int view_h, Vec2 pos, Vec2 dir, double fov);
