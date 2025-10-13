#ifndef VECTORS_H
#define VECTORS_H

#define ABSOLUTE(x) ((x) < 0 ? -(x) : (x))

typedef struct {
  double x;
  double y;
} Vec2;

// Rotate vector by angle (radians)
Vec2 vec2_add(Vec2 a, Vec2 b);
// Subtraction: c = a - b
Vec2 vec2_sub(Vec2 a, Vec2 b);
// Scale: v * s
Vec2 vec2_scale(Vec2 v, float s);
// Dot product: a · b
double vec2_dot(Vec2 a, Vec2 b);
// Length (magnitude) of vector
double vec2_length(Vec2 v);
// Normalize vector (make length = 1)
Vec2 vec2_normalize(Vec2 v);
// Distance between two points
double vec2_distance(Vec2 a, Vec2 b);
// Rotate vector by angle (radians)
Vec2 vec2_rotate(Vec2 v, double angle);
// Multiply vector by scalar
Vec2 vec2_mul_scalar(Vec2 v, double s);
// Componentwise inverse magnitude (composition)
Vec2 vec2_comp(Vec2 v);

#endif
