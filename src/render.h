#ifndef ENGINE_RENDER_H
#define ENGINE_RENDER_H

#include "camera.h"
#include "level.h"
#include "utils.h"

enum
{
  CULL_NONE,
  CULL_BACK,
  CULL_FRONT,
};

enum
{
  RENDER_PIXEL_BUFFER = (1 << 0),
  RENDER_DEPTH_BUFFER = (1 << 1),
};

extern void render_init (const char *const, u32, u32, u32);
extern void render_quit ();

extern void render_clear (u8);
extern void render_present (i64);

extern void draw_point (f32, f32, u32, f32);
extern void draw_verline (f32, f32, f32, u32, f32);

extern void draw_level (level_t, camera_t);

#endif // ENGINE_RENDER_H

