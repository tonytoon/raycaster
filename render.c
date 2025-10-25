#include "render.h"
#include "collision.h"
#include "math.h"
#include "player.h"
#include "sdlstuff.h"
#include "vectors.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern World world;

static inline uint32_t rgbpixel(uint8_t r, uint8_t g, uint8_t b) {
  return 0xff000000 | (uint32_t)r | ((uint32_t)g) << 8 | ((uint32_t)b << 16);
}

static inline uint32_t adjust_brightness(uint32_t color, float brightness) {
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

static inline float ray_intersects_wall(Ray ray, Wall wall) {
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

#define MAX_SLICES_PER_COLUMN 64

typedef struct Wallslice {
  int top;
  int bottom;
  int wall_idx;
  int sector_idx;
  float distance;
  bool is_portal; // true if slice is a portal
} Wallslice;

#define TOP 0
#define MIDDLE 1
#define BOTTOM 2

Wallslice make_wallslice(int wall_idx, int sector_idx, float distance,
                         int column, float view_h, float pos_z) {
  Sector *sector = &world.sectors[sector_idx];
  Wall *wall = &world.walls[wall_idx];

  float ceiling = sector->ceiling_height;
  float floor = sector->floor_height;

  // Project ceiling and floor positions
  Wallslice slice;
  if (wall->neighbor == -1) {
    int ceiling_y = (int)((view_h / 2.0f) -
                          (ceiling - pos_z) / distance * (view_h / 32.0f));
    int floor_y =
        (int)((view_h / 2.0f) - (floor - pos_z) / distance * (view_h / 32.0f));

    if (ceiling_y < 0)
      ceiling_y = 0;
    if (floor_y >= view_h)
      floor_y = view_h - 1;

    slice = (Wallslice){.sector_idx = sector_idx,
                        .distance = distance,
                        .wall_idx = wall_idx,
                        .is_portal = (wall->neighbor != -1),
                        .top = ceiling_y,
                        .bottom = floor_y};
  } else {
    Sector *neighbor = &world.sectors[wall->neighbor];
    float neighbor_ceiling = neighbor->ceiling_height;
    float neighbor_floor = neighbor->floor_height;
    int ceiling_y =
        (int)((view_h / 2.0f) - (fminf(ceiling, neighbor_ceiling) - pos_z) /
                                    distance * (view_h / 32.0f));
    int floor_y =
        (int)((view_h / 2.0f) - (fmaxf(floor, neighbor_floor) - pos_z) /
                                    distance * (view_h / 32.0f));
    if (ceiling_y < 0)
      ceiling_y = 0;
    if (floor_y >= view_h)
      floor_y = view_h - 1;
    slice = (Wallslice){.sector_idx = sector_idx,
                        .distance = distance,
                        .wall_idx = wall_idx,
                        .is_portal = true,
                        .top = ceiling_y,
                        .bottom = floor_y};
  }

  return slice;
}

int cast_ray_column(Ray ray, float fov, int column, int starting_sector,
                    Wallslice *slices) {
  int view_h = VIEWHEIGHT;
  int num_slices = 0;
  int sector_idx = starting_sector;
  const int MAX_ITER = 100;
  int iter_count = 0;

  int nearest_wall = -1;
  float nearest_dist = HUGE_VAL;
  int draw_sector = -1;
  int last_sector = -1; // start with none

  while (sector_idx != -1 && iter_count < MAX_ITER) {
    iter_count++;
    Sector *sector = &world.sectors[sector_idx];

    int hit_wall = -1;
    float hit_dist = HUGE_VAL;
    int hit_portal = -1;

    // find nearest wall/portal in this sector
    for (int i = 0; i < sector->wall_count; i++) {
      Wall *wall = &world.walls[sector->wall_start + i];
      float d = ray_intersects_wall(ray, *wall);
      if (d >= 0.0f && d < hit_dist) {
        // Ignore portal we just came from
        if (wall->neighbor != last_sector || wall->neighbor == -1) {
          hit_dist = d;
          hit_wall = sector->wall_start + i;
          hit_portal = wall->neighbor;
        }
      }
    }
    if (hit_wall == -1)
      break;
    slices[num_slices++] = make_wallslice(hit_wall, sector_idx, hit_dist,
                                          column, VIEWHEIGHT, 16.0f);
    // Portal: traverse neighbor
    if (hit_portal != -1) {
      last_sector = sector_idx;
      sector_idx = hit_portal;
    } else {
      break;
    }
  }
  return num_slices;
}

void draw_slices(uint32_t *buffer, int column, Wallslice *slices,
                 int num_slices) {

  // Simple bubble sort (num_slices is tiny, so it's fine)
  for (int i = 0; i < num_slices - 1; i++) {
    for (int j = i + 1; j < num_slices; j++) {
      if (slices[i].distance < slices[j].distance) {
        Wallslice tmp = slices[i];
        slices[i] = slices[j];
        slices[j] = tmp;
      }
    }
  }

  for (int i = 0; i < num_slices; i++) {

    Wallslice *s = &slices[i];

    // Get wall color and apply brightness based on distance
    // uint32_t pixel = adjust_brightness(
    //    world.walls[s->wall_idx].middle_rgb_color, 1.0f / s->distance);
    uint32_t pixel = 0;
    // Clamp to screen height

    if (!(s->is_portal)) {
      // pixel = adjust_brightness(world.walls[s->wall_idx].upper_rgb_color,
      //                           1.0f / s->distance);
      // for (int y = 0; y < s->top; y++) {
      //   buffer[y * VIEWWIDTH + column] = pixel;
      // }

      int top = s->top;
      int bottom = s->bottom;

      if (top < 0)
        top = 0;
      if (bottom >= VIEWHEIGHT)
        bottom = VIEWHEIGHT - 1;

      pixel = adjust_brightness(world.walls[s->wall_idx].middle_rgb_color,
                                1.0f / s->distance);

      for (int y = top; y <= bottom; y++) {
        buffer[y * VIEWWIDTH + column] = pixel;
      }

      // pixel = adjust_brightness(world.walls[s->wall_idx].lower_rgb_color,
      //                           1.0f / s->distance);
      // for (int y = s->bottom; y <= VIEWHEIGHT - 1; y++) {
      //   buffer[y * VIEWWIDTH + column] = pixel;
      // }
      //}
      //}
    }
  }
}
void render_sector(uint32_t *buffer, int column_start, int column_end,
                   int view_h, Vec2 pos, Vec2 dir, float fov,
                   int starting_sector) {

  float plane_length = tan(fov / 2);
  Vec2 plane = {-dir.y * plane_length, dir.x * plane_length};
  const float pos_z = 16.0f; // camera height

  for (int column = column_start; column < column_end; column++) {
    Wallslice *slices = malloc(sizeof(Wallslice) * MAX_SLICES_PER_COLUMN);
    int num_slices = 0;

    float cameraX = 2.0f * column / VIEWWIDTH - 1.0f;
    Ray ray = {pos, vec2_add(dir, vec2_scale(plane, cameraX))};
    num_slices = cast_ray_column(ray, fov, column, starting_sector, slices);
    draw_slices(buffer, column, slices, num_slices);
  }
}
/*
void render_sector_old(uint32_t *buffer, int column_start, int column_end,
                       int view_h, Vec2 pos, Vec2 dir, float fov,
                       int starting_sector) {

  float plane_length = tan(fov / 2);
  Vec2 plane = {-dir.y * plane_length, dir.x * plane_length};
  const float pos_z = 16.0f; // camera height

  for (int column = column_start; column < column_end; column++) {
    Wallslice slices[MAX_SLICES_PER_COLUMN];
    int num_slices = 0;

    float cameraX = 2.0f * column / VIEWWIDTH - 1.0f;
    Ray ray = {pos, vec2_add(dir, vec2_scale(plane, cameraX))};

    int sector_idx = starting_sector;
    const int MAX_ITER = 100;
    int iter_count = 0;

    int nearest_wall = -1;
    float nearest_dist = HUGE_VAL;
    int draw_sector = -1;

    int last_sector = -1; // start with none
    while (sector_idx != -1 && iter_count < MAX_ITER) {
      iter_count++;
      Sector *sector = &world.sectors[sector_idx];

      int hit_wall = -1;
      float hit_dist = HUGE_VAL;
      int hit_portal = -1;

      for (int i = 0; i < sector->wall_count; i++) {
        Wall *wall = &world.walls[sector->wall_start + i];
        float d = ray_intersects_wall(ray, *wall);
        if (d >= 0.0f && d < hit_dist) {
          // Ignore portal we just came from
          if (wall->neighbor != last_sector || wall->neighbor == -1) {
            hit_dist = d;
            hit_wall = sector->wall_start + i;
            hit_portal = wall->neighbor;
          }
        }
      }

      if (hit_wall == -1)
        break;

      if (hit_portal == -1) {
        nearest_wall = hit_wall;
        nearest_dist = hit_dist;
        draw_sector = sector_idx;
        break;
      } else {
        // Portal: traverse neighbor
        last_sector = sector_idx;
        sector_idx = hit_portal;
      }
    }

    if (nearest_wall != -1) {
      Sector *sector = &world.sectors[draw_sector];

      float floor = sector->floor_height;
      float ceiling = sector->ceiling_height;
      int drawTop = (int)((view_h / 2.0f) -
                          (ceiling - pos_z) / nearest_dist * (view_h
/ 32.0f)); int drawBottom = (int)((view_h / 2.0f) - (floor - pos_z) /
nearest_dist * (view_h / 32.0f));

      // Clamp to screen
      if (drawTop < 0)
        drawTop = 0;
      if (drawBottom >= view_h)
        drawBottom = view_h - 1;

      slices[num_slices++] = (Wallslice){.top = drawTop,
                                         .bottom = drawBottom,
                                         .wall_idx = nearest_wall,
                                         .sector_idx = draw_sector,
                                         .distance = nearest_dist};
      uint32_t pixel = adjust_brightness(
          world.walls[nearest_wall].middle_rgb_color, 1 / nearest_dist);

      for (int y = drawTop; y <= drawBottom; y++) {
        buffer[y * VIEWWIDTH + column] = pixel;
      }
      printf("numslices: %d\n", num_slices);
      for (int i = 0; i < num_slices; i++) {
        Wallslice *s = &slices[i];
        uint32_t pixel = adjust_brightness(
            world.walls[s->wall_idx].middle_rgb_color, 1 / s->distance);
        for (int y = s->top; y <= s->bottom; y++) {
          buffer[y * VIEWWIDTH + column] = pixel;
        }
      }
    }
  }
} */
