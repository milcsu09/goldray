// #include <stdio.h>
// #include "thread.h"

#include <SDL2/SDL.h>
#include "level.h"
#include "render.h"

int main() {
  render_init("Raycast", 800, 600, 1);

  level_t level = {0};
  camera_t camera = {.pos={5, 0, 1}, .yaw=0, .fov=RAD(90)};

  level_append_wall(&level, (wall_t){{10, 0}, {10, 5}, 0, 2});
  level_append_plane(&level, (plane_t){{0, 0}, {10, 10}, 0, 0});
  
  for (u32 i = 0; i < 500; ++i) {
    level_append_plane(&level,
        (plane_t){{0, 0}, {1, 1}, 3 + i * 0.01, CULL_BACK});
  }

  u32 lastTime = SDL_GetTicks();
  u32 frame = 0, last_fps = 0;
  while (TRUE) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        goto quit;
      default:
        break;
      }
    }

    const u8 *keys = SDL_GetKeyboardState(NULL);

    u32 currentTime = SDL_GetTicks();
    if (currentTime - lastTime >= 1000.0f / 60.0f) {
      camera_move(&camera, keys);
      lastTime = currentTime;
    }

    render_clear(RENDER_PIXEL_BUFFER | RENDER_DEPTH_BUFFER);

    draw_level(level, camera);

    render_present(SDL_FLIP_NONE);

    frame++;
    if (currentTime - last_fps >= 1000) {
      printf("%d\n", frame);
      frame = 0;
      last_fps = currentTime;
    }
  }

quit:
  render_quit();

  return 0;
}

