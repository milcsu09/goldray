#ifndef ENGINE_THREAD_H
#define ENGINE_THREAD_H

#include "utils.h"

#ifdef _WIN32
  #include <windows.h>
  typedef DWORD thread_return_t;
  typedef HANDLE thread_t;
  #define THREAD_CALL WINAPI
#else
  #include <pthread.h>
  #include <unistd.h>
  typedef void *thread_return_t;
  typedef pthread_t thread_t;
  #define THREAD_CALL
#endif

typedef thread_return_t (*thread_func_t)(void *);

extern i32 thread_create(thread_t *, thread_func_t, void *);
extern i32 thread_join(thread_t);
extern i32 get_cpu_cores();

#endif // ENGINE_THREAD_H

