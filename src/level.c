#include "level.h"
#include "render.h"

void
level_append_wall (level_t *level, wall_t wall)
{ // Append a wall to 'level'
  level->walls[level->walls_sz++] = wall;
}

void
level_append_plane (level_t *level, plane_t plane)
{ // Append a plane to 'level'
  level->planes[level->planes_sz++] = plane;
}

void
level_append_block (level_t *level, v2 start, v2 end, f32 elevation,
                    f32 height, u8 culling)
{ // Append a block of walls and planes to 'level'
  level_append_wall (level, (wall_t){ { end.x, start.y },
                                      { start.x, start.y },
                                      elevation,
                                      height,
                                      culling });
  level_append_wall (level, (wall_t){ { end.x, end.y },
                                      { end.x, start.y },
                                      elevation,
                                      height,
                                      culling });
  level_append_wall (level, (wall_t){ { start.x, end.y },
                                      { end.x, end.y },
                                      elevation,
                                      height,
                                      culling });
  level_append_wall (level, (wall_t){ { start.x, start.y },
                                      { start.x, end.y },
                                      elevation,
                                      height,
                                      culling });

  const u8 plane_cull_bottom =
    (culling == CULL_BACK) ? CULL_FRONT : CULL_BACK;
  const u8 plane_cull_top =
    (culling == CULL_BACK) ? CULL_BACK : CULL_FRONT;

  level_append_plane (
      level, (plane_t){ start, end, elevation,
                        (!culling) ? 0 : plane_cull_bottom});
  level_append_plane (
      level, (plane_t){ start, end, elevation + height,
                        (!culling) ? 0 : plane_cull_top});
}

