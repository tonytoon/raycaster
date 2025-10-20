#include "collision.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static inline int segments_intersect(Vec2 p1, Vec2 p2, Vec2 q1, Vec2 q2) {
  Vec2 r = vec2_sub(p2, p1);
  Vec2 s = vec2_sub(q2, q1);
  double rxs = vec2_cross(r, s);
  if (rxs == 0.0)
    return 0; // parallel

  Vec2 qp = vec2_sub(q1, p1);
  double t = vec2_cross(qp, s) / rxs;
  double u = vec2_cross(qp, r) / rxs;

  return (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0);
}

static inline int circle_intersects_segment(Vec2 pos, float radius, Vec2 a,
                                            Vec2 b, Vec2 *push_out) {
  Vec2 ab = vec2_sub(b, a);
  Vec2 ap = vec2_sub(pos, a);

  float ab_len2 = vec2_dot(ab, ab);
  if (ab_len2 == 0.0) {
    float dist = vec2_length(ap);
    if (dist < radius && push_out) {
      Vec2 normal = vec2_normalize(ap);
      *push_out = vec2_scale(normal, radius - dist);
    }
    return dist < radius;
  }

  // Project player position onto wall
  float t = vec2_dot(ap, ab) / ab_len2;
  if (t < 0.0)
    t = 0.0;
  if (t > 1.0)
    t = 1.0;

  Vec2 closest = vec2_add(a, vec2_scale(ab, t));
  Vec2 diff = vec2_sub(pos, closest);
  float dist = vec2_length(diff);

  if (dist < radius) {
    if (push_out && dist > 1e-6) {
      Vec2 normal = vec2_scale(diff, 1.0 / dist);
      *push_out = vec2_scale(normal, radius - dist);
    } else if (push_out) {
      *push_out = (Vec2){0, 0};
    }
    return 1;
  }

  return 0;
}

//
// pass the new (intended) position, the previous position, the player's radius,
// and the sector the player is presently located in. Returns the new player
// position after correcting for wall collisions.
// TODO add sliding against walls
//
Vec2 collide(Vec2 pos, Vec2 prev_pos, float radius, int sector_idx) {
  bool *visited = calloc(world.num_sectors, sizeof(bool));
  int *stack = malloc(sizeof(int) * world.num_sectors);
  int stack_size = 0;

  // Start with the last known sector
  stack[stack_size++] = sector_idx;
  Vec2 new_pos = pos;
  Vec2 vel;
  while (stack_size > 0) {
    int sector_idx_curr = stack[--stack_size];
    if (visited[sector_idx_curr])
      continue;
    visited[sector_idx_curr] = true;

    Sector *s = &world.sectors[sector_idx_curr];

    for (int i = 0; i < s->wall_count; i++) {
      Wall *w = &world.walls[s->wall_start + i];
      if (w->neighbor == -1) { // solid wall
        // printf("Testing wall: %d\n", s->wall_start + i);
        Vec2 push;
        if (circle_intersects_segment(new_pos, radius, w->a, w->b, &push)) {

          new_pos = vec2_add(new_pos, push);
        }
      } else { // portal
        if (circle_intersects_segment(pos, radius, w->a, w->b, NULL)) {
          if (!visited[w->neighbor]) {
            stack[stack_size++] = w->neighbor;
          }
        }
      }
    }
  }

  free(stack);
  free(visited);
  return new_pos;
}

// simple wrapper in case movement gets more complicated
Vec2 move_player(Vec2 pos, Vec2 prev_pos, float radius, int sector_idx) {

  return collide(pos, prev_pos, radius, sector_idx);
}

//
// get the cross product of our location and each wall
// in our last known sector. if they're all positive
// then we're in that sector. otherwise try every sector
// we could reach from that sector (player moved between
// sectors). If that too fails, we give up and just try
// every sector. (They probably moved really fast or
// teleported.
//

int find_sector(Vec2 pos, int last_known_sector) {
  // Check current sector first
  if (point_in_sector(pos, last_known_sector)) {
    return last_known_sector;
  }

  // Check adjacent sectors via portal walls
  Sector *s = &world.sectors[last_known_sector];
  for (int i = 0; i < s->wall_count; i++) {
    Wall *w = &world.walls[s->wall_start + i];
    if (w->neighbor != -1) { // portal wall
      if (point_in_sector(pos, w->neighbor)) {
        return w->neighbor;
      }
    }
  }

  // Fallback: check all sectors
  for (int i = 0; i < world.num_sectors; i++) {
    if (i == last_known_sector)
      continue; // already checked
    if (point_in_sector(pos, i)) {
      return i;
    }
  }

  // Not found
  return -1;
}

// Returns 1 if pos is inside the sector at last_known_sector, 0 otherwise
int point_in_sector(Vec2 pos, int last_known_sector) {
  Sector *s = &world.sectors[last_known_sector];
  int n = s->wall_count;
  if (n < 3)
    return 0; // not a polygon

  for (int i = 0; i < n; i++) {
    Wall *w = &world.walls[s->wall_start + i];
    Vec2 ab = vec2_sub(w->b, w->a);
    Vec2 ap = vec2_sub(pos, w->a);
    float cross = vec2_cross(ab, ap);

    // For CCW polygon, point is outside if cross < 0
    if (cross < 0)
      return 0;
  }

  return 1; // inside all edges
}
