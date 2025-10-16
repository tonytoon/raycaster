#include "raycaster.h"
#include "math.h"
#include "sector.h"
#include <stdio.h>
#include <string.h>

uint32_t rgbpixel(uint8_t r, uint8_t g, uint8_t b) {
  return 0xff000000 | (uint32_t)r | ((uint32_t)g) << 8 | ((uint32_t)b << 16);
}

uint32_t adjust_brightness(uint32_t color, float brightness) {
  if (brightness > 1.0)
    brightness = 1.0;
  uint8_t a = (color >> 24) & 0xFF;
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;

  r = (uint8_t)(r * brightness);
  g = (uint8_t)(g * brightness);
  b = (uint8_t)(b * brightness);

  return (a << 24) | (r << 16) | (g << 8) | b;
}

double ray_intersects_wall(Ray ray, Wall wall) {
  Vec2 r = ray.dir;
  Vec2 s = vec2_sub(wall.b, wall.a);
  Vec2 qp = vec2_sub(wall.a, ray.origin);

  double rxs = vec2_cross(r, s);
  if (rxs == 0.0f)
    return -1.0f; // parallel

  double t = vec2_cross(qp, s) / rxs;
  double u = vec2_cross(qp, r) / rxs;

  if (t >= 0.0f && u >= 0.0f && u <= 1.0f) {
    return t; // distance along ray
  }
  return -1.0f; // no intersection
}

void render_sector_untextured(uint32_t *buffer, World *world, int view_w,
                              int view_h, Vec2 pos, Vec2 dir, double fov) {

  double plane_length = tan(fov / 2);
  Vec2 plane = {-dir.y * plane_length, dir.x * plane_length};

  memset(buffer, 0, view_w * view_h * sizeof(uint32_t));
  for (int column = 0; column < view_w; column++) {
    int nearest_wall = -1;
    double nearest_dist = HUGE_VAL;
    double angle = -fov / 2 + fov * column / view_w;
    double cameraX = 2.0 * column / view_w - 1.0;
    Ray ray = {pos, vec2_add(dir, vec2_scale(plane, cameraX))};
    Sector sector = world->sectors[0];
    for (int i = 0; i != sector.wall_count; i++) {
      double d = ray_intersects_wall(ray, world->walls[sector.wall_start + i]);
      if (d >= 0.0f && d < nearest_dist) {
        nearest_dist = d;
        nearest_wall = i;
      }
    }

    int stripHeight = (int)view_h / nearest_dist;

    int drawStart = -stripHeight / 2 + view_h / 2;
    if (drawStart < 0)
      drawStart = 0;
    int drawEnd = stripHeight / 2 + view_h / 2;
    if (drawEnd >= view_h)
      drawEnd = view_h - 1;

    uint32_t pixel = adjust_brightness(
        world->walls[sector.wall_start + nearest_wall].rgb_color,
        16 / nearest_dist);
    for (int y = drawStart; y != drawEnd; y++) {
      buffer[y * view_w + column] = pixel;
    }
  }
}
