#include "map.h"
#include "vectors.h"
#include <stdint.h>

void render_untextured(uint32_t *buffer, Map *map, int view_w, int view_h,
                       Vec2 pos, Vec2 dir, Vec2 viewplane);
