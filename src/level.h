#ifndef LEVEL_WORLD_H
#define LEVEL_WORLD_H

#include "utils.h"

#define MAX_WALLS_SZ 1024
#define MAX_PLANES_SZ 1024

typedef struct
{
  v2 start, end;
  f32 elevation;
  f32 height;
  u8 culling;
} wall_t;

typedef struct
{
  v2 start, end;
  f32 elevation;
  u8 culling;
} plane_t;

typedef struct
{
  wall_t walls[MAX_WALLS_SZ];
  plane_t planes[MAX_PLANES_SZ];
  u32 walls_sz, planes_sz;
} level_t;

extern void level_append_wall (level_t *, wall_t);
extern void level_append_plane (level_t *, plane_t);
extern void level_append_block (level_t *, v2, v2, f32, f32, u8);

#endif // LEVEL_WORLD_H

