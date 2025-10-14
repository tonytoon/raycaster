#include "raycaster.h"
#include <stdio.h>
#include <string.h>
#define THINWALLS
uint32_t rgbpixel(uint8_t r, uint8_t g, uint8_t b) {
  return 0xff000000 | (uint32_t)r | ((uint32_t)g) << 8 | ((uint32_t)b << 16);
}

void render_untextured(uint32_t *buffer, Map *map, int view_w, int view_h,
                       Vec2 pos, Vec2 dir, Vec2 viewplane) {

  memset(buffer, 0, view_w * view_h * sizeof(uint32_t));

  for (int column = 0; column < view_w; column++) {
    // calculate ray position and direction
    Vec2 ray;
    double camera_x =
        2 * column / (double)view_w - 1; // x-coordinate in camera space
    ray = vec2_add(dir, vec2_mul_scalar(viewplane, camera_x));

    // which box of the map we're in
    int map_x = (int)pos.x;
    int map_y = (int)pos.y;

    // length of ray from current position to next x or y-side
    Vec2 side_dist;

    // length of ray from one x or y-side to next x or y-side
    Vec2 dda = vec2_comp(ray);
    // dda.x = (ray.x == 0) ? 1e30 : ABSOLUTE(1 / ray.x);
    // dda.y = (ray.y == 0) ? 1e30 : ABSOLUTE(1 / ray.y);
    double perpWallDist;

    // what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    int hit = 0; // was there a wall hit?
    int side;    // was a NS or a EW wall hit?

    // calculate step and initial sideDist
    if (ray.x < 0) {
      stepX = -1;
      side_dist.x = (pos.x - map_x) * dda.x;
    } else {
      stepX = 1;
      side_dist.x = (map_x + 1.0 - pos.x) * dda.x;
    }

    if (ray.y < 0) {
      stepY = -1;
      side_dist.y = (pos.y - map_y) * dda.y;
    } else {
      stepY = 1;
      side_dist.y = (map_y + 1.0 - pos.y) * dda.y;
    }

    // perform DDA
    while (hit == 0) {
      // jump to next map square, either in x-direction, or in y-direction
      if (side_dist.x < side_dist.y) {
        side_dist.x += dda.x;
        map_x += stepX;
        side = 0;
      } else {
        side_dist.y += dda.y;
        map_y += stepY;
        side = 1;
      }
      // Check if ray has hit a wall
#ifndef THINWALLS
      if (map->tiles[map_y * map->w + map_x].walls > 0) {
        hit = 1;
      }
    }
#else
      if (side == 0) {
        if (stepX < 0) {
          int walls = map->tiles[map_y * map->w + (map_x + 1)].walls;
          hit |= (walls & W_WALL);
        } else {
          int walls = map->tiles[map_y * map->w + (map_x - 1)].walls;
          hit |= (walls & E_WALL);
        }
      } else if (side == 1) {
        if (stepY < 0) {
          int walls = map->tiles[(map_y + 1) * map->w + map_x].walls;
          hit |= (walls & N_WALL);
        } else {
          int walls = map->tiles[(map_y - 1) * map->w + map_x].walls;
          hit |= (walls & S_WALL);
        }
      }
    }
#endif
    // Calculate distance projected on camera direction (Euclidean distance
    // would give fisheye effect!)
    if (side == 0)
      perpWallDist = (side_dist.x - dda.x);
    else
      perpWallDist = (side_dist.y - dda.y);

    // Calculate height of line to draw on screen
    int lineHeight = (int)view_h / perpWallDist;

    // calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + view_h / 2;
    if (drawStart < 0)
      drawStart = 0;
    int drawEnd = lineHeight / 2 + view_h / 2;
    if (drawEnd >= view_h)
      drawEnd = view_h - 1;

    uint32_t pixel;
    if (side)
      pixel = rgbpixel(0, 255, 0);
    else
      pixel = rgbpixel(0, 255 / 2, 0);

    /*
     * switch (map->tiles[map_y * map->w + map_x].walls) {
    case 1:
      if (side)
        pixel = rgbpixel(255, 0, 0);
      else
        pixel = rgbpixel(255 / 2, 0, 0);
      break;
    case 2:
      if (side)
        pixel = rgbpixel(0, 255, 0);
      else
        pixel = rgbpixel(0, 255 / 2, 0);
      break;
    case 3:
      if (side)
        pixel = rgbpixel(0, 0, 255);
      else
        pixel = rgbpixel(0, 0, 255 / 2);
      break;
    case 4:
      if (side)
        pixel = rgbpixel(255, 255, 255);
      else
        pixel = rgbpixel(255 / 2, 255 / 2, 255 / 2);
      break;
    default:
      if (side)
        pixel = rgbpixel(255, 255, 0);
      else
        pixel = rgbpixel(255 / 2, 255 / 2, 0);
    }*/

    for (int y = drawStart; y != drawEnd; y++) {
      buffer[y * view_w + column] = pixel;
    }
  }
}
