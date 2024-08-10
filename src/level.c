#include "level.h"

void
level_append_wall(level_t *level, wall_t wall)
{ // Append a wall to 'level'
  level->walls[level->walls_sz++] = wall;
}

void
level_append_plane(level_t *level, plane_t plane)
{ // Append a plane to 'level'
  level->planes[level->planes_sz++] = plane;
}

