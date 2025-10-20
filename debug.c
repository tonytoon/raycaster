#include "debug.h"

void debug_draw_player(SDL_Renderer *renderer, Player *player) {
  int scale = 4;
  int px = (int)(player->pos.x * scale);
  int py = (int)(player->pos.y * scale);

  // Player position
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  SDL_Rect r = {px - 2, py - 2, 4, 4};
  SDL_RenderFillRect(renderer, &r);

  // Facing direction
  int dx = (int)(player->dir.x * 10);
  int dy = (int)(player->dir.y * 10);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderDrawLine(renderer, px, py, px + dx, py + dy);

  // Strafe direction (right)
  Vec2 right = vec2_rotate(player->dir, M_PI / 2.0);
  int rx = (int)(right.x * 10);
  int ry = (int)(right.y * 10);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawLine(renderer, px, py, px + rx, py + ry);
}

void debug_draw_world(SDL_Renderer *renderer, World *world, int scale) {
  // Disable blending for solid overlay
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

  for (int s = 0; s < world->num_sectors; s++) {
    Sector *sector = &world->sectors[s];

    for (int i = 0; i < sector->wall_count; i++) {
      Wall *wall = &world->walls[sector->wall_start + i];

      // Choose color: portals in green, solid walls in red
      if (wall->neighbor == -1) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // solid red
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // solid green
      }

      // Draw line scaled for top-down view
      SDL_RenderDrawLine(renderer, (int)(wall->a.x * scale),
                         (int)(wall->a.y * scale), (int)(wall->b.x * scale),
                         (int)(wall->b.y * scale));
    }
  }
}
