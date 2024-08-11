#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <math.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

#define ASSERT(expr, ...)                                            \
  if (!(expr))                                                       \
  exit ((fprintf (stderr, __VA_ARGS__), 1))

#define LOG(...)                                                     \
  ({                                                                 \
    printf ("[%s] ", __func__);                                      \
    printf (__VA_ARGS__);                                            \
  })

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(v, a, b) MAX ((a), MIN ((v), (b)))

#define PI 3.141592653589793238462643383279502984f
#define HALF_PI (PI * 0.5f)

#define RAD(a) ((a) * (PI / 180.0f))
#define DEG(a) ((a) / (PI / 180.0f))

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef struct
{
  f32 x, y;
} v2;

typedef struct
{
  f32 x, y, z;
} v3;

static inline v2
v2_sub (const v2 v0, const v2 v1)
{ // Difference between 'v0' and 'v1'
  return (v2){ v0.x - v1.x, v0.y - v1.y };
}

static inline f32
v2_dist (const v2 v0, const v2 v1)
{ // Distance between 'v0' and 'v1'
  const f32 dx = v1.x - v0.x, dy = v1.y - v0.y;
  return sqrtf (dx * dx + dy * dy);
}

static inline f32
v2_bearing (const v2 v0, const v2 v1)
{ // Angle between 'v0' and 'v1'
  const f32 dx = v1.x - v0.x, dy = v1.y - v0.y;
  return atan2f (dy, dx);
}

#endif // ENGINE_UTILS_H

