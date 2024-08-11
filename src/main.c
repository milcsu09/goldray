#include "level.h"
#include "render.h"
#include <SDL2/SDL.h>

int
main ()
{
  render_init ("Raycast", 800, 600, 1);

  level_t level = { 0 };
  camera_t camera = { .pos = { 1, 1, 1 }, .fov = RAD (90) };

  level_append_block(&level, (v2){0, 0}, (v2){15, 15}, 0, 15, CULL_FRONT);
  level_append_block(&level, (v2){7, 7}, (v2){8, 8}, .5, .5, CULL_BACK);
  level_append_block(&level, (v2){9, 7}, (v2){10, 8}, 2, .5, CULL_BACK);
  level_append_block(&level, (v2){8, 9}, (v2){9, 10}, 1.25, .5, CULL_BACK);
  level_append_block(&level, (v2){5, 6}, (v2){6, 7}, 1.25, .5, CULL_BACK);
  level_append_block(&level, (v2){5, 9}, (v2){6, 10}, 2, .5, CULL_BACK);

  u32 lastTime = SDL_GetTicks ();
  u32 frame = 0, last_fps = 0;
  u32 all_fps = 0, count = 0;
  while (TRUE)
    {
      SDL_Event event;
      while (SDL_PollEvent (&event))
        {
          switch (event.type)
            {
            case SDL_QUIT:
              goto quit;
            default:
              break;
            }
        }

      const u8 *keys = SDL_GetKeyboardState (NULL);

      u32 currentTime = SDL_GetTicks ();
      if (currentTime - lastTime >= 1000.0f / 60.0f)
        {
          camera_move (&camera, keys);

          if (keys[SDL_SCANCODE_PAGEUP])
            {
              camera.fov += RAD (1);
            }
          if (keys[SDL_SCANCODE_PAGEDOWN])
            {
              camera.fov -= RAD (1);
            }

          lastTime = currentTime;
        }

      render_clear (RENDER_PIXEL_BUFFER | RENDER_DEPTH_BUFFER);
      camera.yaw = v2_bearing((v2){camera.pos.x, camera.pos.y},
                              (v2){7.5, 7.5});
      draw_level (level, camera);
      render_present (SDL_FLIP_NONE);

      frame++;
      if (currentTime - last_fps >= 1000)
        {
          all_fps += frame;
          count++;
          printf ("%d, %d\n", frame, all_fps / count);
          frame = 0;
          last_fps = currentTime;
        }
    }

quit:
  render_quit ();

  return 0;
}

