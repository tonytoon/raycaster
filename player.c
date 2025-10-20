#include "player.h"
#include <stdio.h>

#include <stdio.h>

void print_sector_walls(Sector *s, Wall *walls) {
  printf("Sector walls (start=%d, count=%d):\n", s->wall_start, s->wall_count);

  float sum = 0.0f; // signed area
  for (int i = 0; i < s->wall_count; i++) {
    Wall *w = &walls[s->wall_start + i];
    printf("  wall %d: a=(%f,%f), b=(%f,%f), neighbor=%d\n", i, w->a.x, w->a.y,
           w->b.x, w->b.y, w->neighbor);

    Vec2 a = w->a;
    Vec2 b = w->b;
    sum += (b.x - a.x) * (b.y + a.y);
  }

  const float EPSILON = 1e-6f;
  if (sum > EPSILON) {
    printf("  Sector orientation: CW\n");
  } else if (sum < -EPSILON) {
    printf("  Sector orientation: CCW\n");
  } else {
    printf("  Sector orientation: indeterminate / degenerate\n");
  }
}

void handleinput(Input *input, Player *player) {
  // Rotation
  if (input->turn_left) {
    player->dir = vec2_rotate(player->dir, -player->rot);
    player->viewplane = vec2_rotate(player->viewplane, -player->rot);
  }
  if (input->turn_right) {
    player->dir = vec2_rotate(player->dir, player->rot);
    player->viewplane = vec2_rotate(player->viewplane, player->rot);
  }

  // Movement
  Vec2 move = {0, 0};

  if (input->move_forward)
    move = vec2_add(move, player->dir);
  if (input->move_backward)
    move = vec2_sub(move, player->dir);

  if (input->move_left) {
    Vec2 left = {player->dir.y, -player->dir.x}; // 90° left (corrected)
    move = vec2_add(move, left);
  }
  if (input->move_right) {
    Vec2 right = {-player->dir.y, player->dir.x}; // 90° right (corrected)
    move = vec2_add(move, right);
  }

  //  Normalize diagonal movement
  if (move.x != 0 || move.y != 0)
    move = vec2_normalize(move);

  player->current_sector = find_sector(player->pos, player->current_sector);
  // printf("sector: %d\n", player->current_sector);
  //  print_sector_walls(&world.sectors[player->current_sector], world.walls);
  // printf("player x: %f y: %f\n", player->pos.x, player->pos.y);
  Vec2 new_pos = vec2_add(player->pos, vec2_scale(move, player->speed));
  player->pos =
      move_player(new_pos, player->pos, player->radius, player->current_sector);

  player->current_sector = find_sector(player->pos, player->current_sector);
}
