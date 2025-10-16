#include "player.h"
#include <math.h>
#include <stdio.h>

void handleinput(Input *input, Player *player) {
  // rotation
  if (input->turn_left) {
    player->dir = vec2_rotate(player->dir, -player->rot);
    player->viewplane = vec2_rotate(player->viewplane, -player->rot);
  }
  if (input->turn_right) {
    player->dir = vec2_rotate(player->dir, player->rot);
    player->viewplane = vec2_rotate(player->viewplane, player->rot);
  }

  // movement
  Vec2 newpos;
  if (input->move_forward) {
    player->pos =
        vec2_add(player->pos, vec2_mul_scalar(player->dir, player->speed));
  }
  if (input->move_backward) {
    player->pos =
        vec2_add(player->pos, vec2_mul_scalar(player->dir, -(player->speed)));
  }
  if (input->move_left) {
    Vec2 angle = vec2_rotate(player->dir, -(M_PI / 2.00));
    player->pos =
        vec2_add(player->pos, vec2_mul_scalar(angle, (player->speed)));
  }
  if (input->move_right) {
    Vec2 angle = vec2_rotate(player->dir, (M_PI / 2.00));
    player->pos =
        vec2_add(player->pos, vec2_mul_scalar(angle, (player->speed)));
  }
}
