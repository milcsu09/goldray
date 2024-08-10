#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "utils.h"

typedef struct {
  v3 pos;
  f32 fov, yaw;
} camera_t;

extern void camera_move(camera_t *, const u8 *);

#endif // ENGINE_CAMERA_H

