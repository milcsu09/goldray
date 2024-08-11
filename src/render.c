#include "ray.h"
#include "render.h"
#include <SDL2/SDL.h>
#include <omp.h>

static struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  u32 *pixel_buffer;
  f32 *depth_buffer;
  u32 width, height;
} g_state = { 0 };

static inline u32
pack_color (u8 r, u8 g, u8 b)
{ // Pack R, G, B values into u32
  return (r << 16) | (g << 8) | b;
}

void
render_init (const char *const title, u32 width, u32 height,
             u32 pixel_size)
{ // Initalize the main renderer
  ASSERT (SDL_Init (SDL_INIT_VIDEO) == 0, "SDL_Init(): %s\n",
          SDL_GetError ());

  g_state.window = SDL_CreateWindow (title, SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, width,
                                     height, SDL_WINDOW_SHOWN);
  ASSERT (g_state.window, "SDL_CreateWindow(): %s\n",
          SDL_GetError ());

  g_state.renderer = SDL_CreateRenderer (g_state.window, -1,
                                         SDL_RENDERER_ACCELERATED);
  ASSERT (g_state.window, "SDL_CreateRenderer(): %s\n",
          SDL_GetError ());

  g_state.width = width / pixel_size;
  g_state.height = height / pixel_size;

  g_state.texture = SDL_CreateTexture (
      g_state.renderer, SDL_PIXELFORMAT_RGB888,
      SDL_TEXTUREACCESS_STREAMING, g_state.width, g_state.height);
  ASSERT (g_state.texture, "SDL_CreateTexture(): %s\n",
          SDL_GetError ());

  const u32 buffer_sz = g_state.width * g_state.height;
  g_state.pixel_buffer = calloc (buffer_sz, sizeof (u32));
  g_state.depth_buffer = calloc (buffer_sz, sizeof (f32));

  const u32 procs = MAX ((u32) (omp_get_num_procs ()) / 2, 1);
  omp_set_num_threads (procs);
  LOG ("cores (in use): %i\n", procs);
}

void
render_quit ()
{ // Quit the main renderer
  SDL_DestroyTexture (g_state.texture);
  SDL_DestroyRenderer (g_state.renderer);
  SDL_DestroyWindow (g_state.window);
  free (g_state.pixel_buffer);
  free (g_state.depth_buffer);
  SDL_Quit ();
}

void
render_clear (u8 flags)
{ // Clear the given buffers
  const u32 buffer_sz = g_state.width * g_state.height;

  if (flags & RENDER_PIXEL_BUFFER)
    {
      memset (g_state.pixel_buffer, 0, buffer_sz * sizeof (u32));
    }

  for (u32 i = 0; i < buffer_sz; ++i)
    {
      if (flags & RENDER_DEPTH_BUFFER)
        {
          g_state.depth_buffer[i] = 0x400;
        }
    }
}

void
render_present (i64 flags)
{ // Copy 'g_state.pixel_buffer' into 'g_state.texture'
  SDL_UpdateTexture (g_state.texture, NULL, g_state.pixel_buffer,
                     g_state.width * sizeof (u32));
  SDL_RenderCopyEx (g_state.renderer, g_state.texture, NULL, NULL, 0,
                    NULL, flags);
  SDL_RenderPresent (g_state.renderer);
}

inline u32
point_to_index (f32 x, f32 y, u32 stride)
{ // Converts v2 into buffer index
  return (i32)x + (i32)y * stride;
}

static inline u8
point_in_bounds (f32 x, f32 y)
{ // Check if x and y is in valid position
  return (x >= 0 && x < g_state.width)
         && (y >= 0 && y < g_state.height);
}

void
draw_point (f32 x, f32 y, u32 color, f32 depth)
{ // Draw a point
  if (!point_in_bounds (x, y))
    {
      return;
    }

  const u32 index = point_to_index (x, y, g_state.width);
  if (depth < g_state.depth_buffer[index])
    {
      g_state.pixel_buffer[index] = color;
      g_state.depth_buffer[index] = depth;
    }
}

void
draw_verline (f32 x, f32 y0, f32 y1, u32 color, f32 depth)
{ // Draw a vertical line from 'y0' to 'y1'
  for (i32 y = (i32)y0; y < (i32)y1; ++y)
    {
      draw_point (x, y, color, depth);
    }
}

static inline void
draw_level_walls (camera_t camera, level_t level)
{ // Render walls in a given window range
  const v2 look = { cosf (camera.yaw), sinf (camera.yaw) };
  const v2 left = { look.y, -look.x };

  const f32 k = tanf (0.5f * camera.fov);

  const v2 left_end = { look.x + k * left.x, look.y + k * left.y };
  const v2 increment = { -2.0f * k * left.x / g_state.width,
                         -2.0f * k * left.y / g_state.width };

#pragma omp parallel for schedule(guided)
  for (u32 x = 0; x < g_state.width; ++x)
    {
      ray_t ray = { { camera.pos.x, camera.pos.y }, 0 };
      ray.angle = v2_bearing (
          ray.start,
          (v2){
              camera.pos.x + left_end.x + (f32)x * increment.x,
              camera.pos.y + left_end.y + (f32)x * increment.y,
          });

      ray_hit_t hits[level.walls_sz];
      u32 hits_sz = 0;

      for (u32 i = 0; i < level.walls_sz; ++i)
        {
          const wall_t wall = level.walls[i];

          i32 side;
          const v2 point = ray_check (ray, wall, &side);
          if (isnan (point.x) || isnan (point.y))
            {
              continue;
            }

          if (2 - side == wall.culling)
            {
              continue;
            }

          const f32 dist = v2_dist (point, ray.start);
          hits[hits_sz++] = (ray_hit_t){
            .side = side, .dist = dist, .point = point, .wall = wall
          };
        }

      for (u32 i = 0; i < hits_sz; ++i)
        {
          ray_hit_t hit = hits[i];
          const i32 side = hit.side;

          const f32 depth = hit.dist;
          if (depth < 0.1f)
            {
              continue;
            }

          hit.dist *= cosf (camera.yaw - ray.angle);
          const f32 projection_plane
              = g_state.width / (2.0f * tanf (camera.fov / 2.0f));
          const f32 size = (projection_plane / hit.dist) / 2.0f;

          const f32 height = hit.wall.height;

          const f32 elevation = camera.pos.z - hit.wall.elevation;
          const f32 y_center = g_state.height / 2.0f;

          const f32 y0 = y_center - size * height + size * elevation;
          const f32 y1 = y_center + size * elevation;

          const f32 wall_length
              = v2_dist (hit.wall.start, hit.wall.end);
          const f32 hit_start = v2_dist (hit.wall.start, hit.point);

          const u32 texture_width = 64;
          const u32 tx = texture_width * hit_start / wall_length;

          draw_verline (
              x, MAX (y0, 0), MIN (y1, g_state.height),
              pack_color (tx + 32 * side, 0, tx + 32 * side), depth);
        }
    }
}

static inline void
draw_level_planes (camera_t camera, level_t level)
{ // Render given range of planes
  const f32 half_fov = camera.fov / 2.0f;
  const f32 inv_sin_hfov = 1.0f / sinf (half_fov);

#pragma omp parallel for schedule(guided)
  for (u32 i = 0; i < level.planes_sz; ++i)
    {
      const plane_t plane = level.planes[i];

      f32 elevation = camera.pos.z - plane.elevation;
      if ((plane.culling == CULL_BACK && elevation <= 0)
          || (plane.culling == CULL_FRONT && elevation >= 0))
        {
          continue;
        }

      const f32 const_factor
          = (elevation * g_state.width * inv_sin_hfov) / 4.0f;
      const f32 y_center = g_state.height / 2.0f;

      const v2 most_left = { cosf (camera.yaw - half_fov),
                             sinf (camera.yaw - half_fov) };
      const v2 most_right = { cosf (camera.yaw + half_fov),
                              sinf (camera.yaw + half_fov) };

      const i32 y0 = (elevation >= 0) ? y_center : 0;
      const i32 y1 = y0 + y_center + 1;

      for (i32 y = y0; y < y1; ++y)
        {
          const f32 row_dist = const_factor / (y - y_center);

          const v2 left
              = { most_left.x * row_dist, most_left.y * row_dist };
          const v2 right
              = { most_right.x * row_dist, most_right.y * row_dist };

          const f32 step_x = (right.x - left.x) / g_state.width;
          const f32 step_y = (right.y - left.y) / g_state.width;

          f32 floor_x = left.x + camera.pos.x;
          f32 floor_y = left.y + camera.pos.y;

          for (u32 x = 0; x < g_state.width; ++x)
            {
              if (plane.start.x <= floor_x && floor_x < plane.end.x
                  && plane.start.y <= floor_y
                  && floor_y < plane.end.y)
                {
                  const f32 dx = floor_x - plane.start.x;
                  const f32 dy = floor_y - plane.start.y;
                  const f32 tx = (int)((dx - (int)dx) * 64);
                  const f32 ty = (int)((dy - (int)dy) * 64);
                  const f32 depth
                      = v2_dist ((v2){ floor_x, floor_y },
                                 (v2){ camera.pos.x, camera.pos.y });

                  draw_point (x, y - 1, pack_color (tx, ty, tx + ty),
                              depth);
                }

              floor_x += step_x;
              floor_y += step_y;
            }
        }
    }
}

void
draw_level (level_t level, camera_t camera)
{ // Draw a level on multiple CPU cores
  draw_level_walls (camera, level);
  draw_level_planes (camera, level);
}

