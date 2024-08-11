#include "camera.h"
#include <SDL2/SDL_keycode.h>

void
camera_move (camera_t *camera, const u8 *keys)
{
  if (keys[SDL_SCANCODE_LEFT])
    camera->yaw -= RAD (3.5f);

  if (keys[SDL_SCANCODE_RIGHT])
    camera->yaw += RAD (3.5f);

  if (camera->yaw > PI * 2 || camera->yaw < -PI * 2)
    camera->yaw = 0;

  if (keys[SDL_SCANCODE_LSHIFT])
    camera->pos.z -= 0.1;

  if (keys[SDL_SCANCODE_SPACE])
    camera->pos.z += 0.1;

  if (keys[SDL_SCANCODE_S])
    {
      camera->pos.x -= cosf (camera->yaw) * 0.1;
      camera->pos.y -= sinf (camera->yaw) * 0.1;
    }

  if (keys[SDL_SCANCODE_W])
    {
      camera->pos.x += cosf (camera->yaw) * 0.1;
      camera->pos.y += sinf (camera->yaw) * 0.1;
    }

  if (keys[SDL_SCANCODE_A])
    {
      camera->pos.x += cosf (camera->yaw - HALF_PI) * 0.1;
      camera->pos.y += sinf (camera->yaw - HALF_PI) * 0.1;
    }

  if (keys[SDL_SCANCODE_D])
    {
      camera->pos.x -= cosf (camera->yaw - HALF_PI) * 0.1;
      camera->pos.y -= sinf (camera->yaw - HALF_PI) * 0.1;
    }
}

