#ifndef ENGINE_RAY_H
#define ENGINE_RAY_H

#include "level.h"
#include "utils.h"

typedef struct
{
  v2 start;
  f32 angle;
} ray_t;

typedef struct
{
  i32 side;
  f32 dist;
  v2 point;
  wall_t wall;
} ray_hit_t;

extern v2 ray_check (ray_t, wall_t, i32 *);

#endif // ENGINE_RAY_H

