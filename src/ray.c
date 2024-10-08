#include "ray.h"
#include "utils.h"

v2
ray_check (ray_t ray, wall_t wall, i32 *side)
{ // Check intersection point and side between two lines
  const f32 x1 = wall.start.x, y1 = wall.start.y;
  const f32 x2 = wall.end.x, y2 = wall.end.y;
  const f32 x3 = ray.start.x, y3 = ray.start.y;
  const f32 x4 = x3 + cosf (ray.angle), y4 = y3 + sinf (ray.angle);

  const f32 d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
  if (d == 0)
    return (v2){ NAN, NAN };

  const f32 t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / d;
  const f32 u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / d;

  if (t > 0.0f && t < 1.0f && u > 0.0f)
    {
      const v2 wall_dir = (v2){ x2 - x1, y2 - y1 };
      const v2 ray_dir = (v2){ x4 - x3, y4 - y3 };
      const v2 wall_normal = (v2){ -wall_dir.y, wall_dir.x };
      *side = (wall_normal.x * ray_dir.x + wall_normal.y * ray_dir.y)
              > 0;

      return (v2){ x1 + t * (x2 - x1), y1 + t * (y2 - y1) };
    }

  return (v2){ NAN, NAN };
}

