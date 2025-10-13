#include "vectors.h"
#include "math.h"

// Addition: c = a + b
Vec2 vec2_add(Vec2 a, Vec2 b) { return (Vec2){a.x + b.x, a.y + b.y}; }

// Subtraction: c = a - b
Vec2 vec2_sub(Vec2 a, Vec2 b) { return (Vec2){a.x - b.x, a.y - b.y}; }

// Scale: v * s
Vec2 vec2_scale(Vec2 v, float s) { return (Vec2){v.x * s, v.y * s}; }

// Dot product: a · b
double vec2_dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }

// Length (magnitude) of vector
double vec2_length(Vec2 v) { return sqrtf(v.x * v.x + v.y * v.y); }

// Normalize vector (make length = 1)
Vec2 vec2_normalize(Vec2 v) {
  float len = vec2_length(v);
  if (len == 0)
    return (Vec2){0, 0};
  return vec2_scale(v, 1.0f / len);
}

// Distance between two points
double vec2_distance(Vec2 a, Vec2 b) { return vec2_length(vec2_sub(a, b)); }

// Rotate vector by angle (radians)
Vec2 vec2_rotate(Vec2 v, double angle) {
  float c = cos(angle);
  float s = sin(angle);
  return (Vec2){v.x * c - v.y * s, v.x * s + v.y * c};
}

// Multiply vector by scalar
Vec2 vec2_mul_scalar(Vec2 v, double s) { return (Vec2){v.x * s, v.y * s}; }

// Componentwise reciprocal of vector (composition)
Vec2 vec2_comp(Vec2 v) {
  return (Vec2){(v.x == 0) ? 1e30 : ABSOLUTE(1.0 / v.x),
                (v.y == 0) ? 1e30 : ABSOLUTE(1.0 / v.y)};
}
