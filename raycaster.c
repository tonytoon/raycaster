#include "raycaster.h"
#include "collision.h"
#include "math.h"
#include "player.h"
#include "sdlstuff.h"
#include "vectors.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern World world;

uint32_t rgbpixel(uint8_t r, uint8_t g, uint8_t b) {
  return 0xff000000 | (uint32_t)r | ((uint32_t)g) << 8 | ((uint32_t)b << 16);
}

uint32_t adjust_brightness(uint32_t color, float brightness) {
  if (brightness > 1.0)
    brightness = 1.0f;
  uint8_t a = (color >> 24) & 0xFF;
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;

  r = (uint8_t)(r * brightness);
  g = (uint8_t)(g * brightness);
  b = (uint8_t)(b * brightness);

  return (a << 24) | (r << 16) | (g << 8) | b;
}

float ray_intersects_wall(Ray ray, Wall wall) {
  Vec2 r = ray.dir;
  Vec2 s = vec2_sub(wall.b, wall.a);
  Vec2 qp = vec2_sub(wall.a, ray.origin);

  float rxs = vec2_cross(r, s);
  if (rxs == 0.0f)
    return -1.0f; // parallel

  float t = vec2_cross(qp, s) / rxs;
  float u = vec2_cross(qp, r) / rxs;

  if (t >= 0.0f && u >= 0.0f && u <= 1.0f) {
    return t; // distance along ray
  }
  return -1.0f; // no intersection
}

void render_sector(uint32_t *buffer, int column_start, int column_end,
                   int view_h, Vec2 pos, Vec2 dir, float fov,
                   int starting_sector) {

  float plane_length = tan(fov / 2);
  Vec2 plane = {-dir.y * plane_length, dir.x * plane_length};

  for (int column = column_start; column < column_end; column++) {
    int nearest_wall = -1;
    float nearest_dist = HUGE_VAL;
    float cameraX = 2.0 * column / VIEWWIDTH - 1.0;
    Ray ray = {pos, vec2_add(dir, vec2_scale(plane, cameraX))};
    int sector_idx = starting_sector;
    const int MAX_ITER = 100;
    int iter_count = 0;
    int last_portal = -1;
    while (sector_idx != -1) {

      iter_count++;
      if (iter_count > MAX_ITER) {
        printf("Possible infinite loop detected.\n");
        break;
      }
      Sector *sector = &world.sectors[sector_idx];
      /*printf("Current sector: %d, Rendering sector: %d\n", starting_sector,
             sector_idx);*/
      int hit_wall = -1;
      float hit_dist = HUGE_VAL;
      int hit_portal = -1;

      for (int i = 0; i != sector->wall_count; i++) {
        Wall *wall = &world.walls[sector->wall_start + i];
        float d = ray_intersects_wall(ray, *wall);
        if (d >= 0.0f && d < hit_dist) {
          // printf("last_portal: %d, hit_portal: %d\n", last_portal,
          // hit_portal);
          if (wall->neighbor != last_portal || wall->neighbor == -1) {
            hit_dist = d;
            hit_wall = sector->wall_start + i;
            hit_portal = wall->neighbor;
          }
        }
      }

      // never hit anything
      if (hit_wall == -1) {
        sector_idx = -1;
      }
      // hit a wall that isn't a portal
      else if (hit_portal == -1 || hit_portal == last_portal) {
        nearest_wall = hit_wall;
        nearest_dist = hit_dist;
        sector_idx = -1;
      }
      // traverse portal
      last_portal = sector_idx;
      sector_idx = hit_portal;
    }

    // printf("drawing sector: %d, wall: %d\n", sector_idx, nearest_wall);
    if (nearest_wall != -1) {
      int stripHeight = (int)view_h / nearest_dist;

      int drawStart = -stripHeight / 2 + view_h / 2;
      if (drawStart < 0)
        drawStart = 0;
      int drawEnd = stripHeight / 2 + view_h / 2;
      if (drawEnd >= view_h)
        drawEnd = view_h - 1;

      uint32_t pixel = (adjust_brightness(world.walls[nearest_wall].rgb_color,
                                          1 / nearest_dist));

      for (int y = drawStart; y != drawEnd; y++) {
        buffer[y * VIEWWIDTH + column] = pixel;
      }
    }
  }
}
