#include "vectors.h"
#include <stdint.h>
#include <stdio.h>

void render_untextured(uint32_t *buffer, int *map, int map_w, int map_h,
                       int view_w, int view_h, Vec2 pos, Vec2 dir,
                       Vec2 viewplane);
