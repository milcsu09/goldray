#include "thread.h"

i32
thread_create(thread_t *thread, thread_func_t func, void *arg)
{ // Create a thread
#ifdef _WIN32
  *thread = CreateThread(NULL, 0, func, arg, 0, NULL);
  return (*thread != NULL) ? 0 : -1;
#else
  return pthread_create(thread, NULL, func, arg);
#endif
}

i32
thread_join(thread_t thread)
{ // Join a thread
#ifdef _WIN32
  WaitForSingleObject(thread, INFINITE);
  return CloseHandle(thread) ? 0 : -1;
#else
  return pthread_join(thread, NULL);
#endif
}

i32
get_cpu_cores()
{ // Query the number of CPU cores available
#ifdef _WIN32
  SYSTEM_INFO sys_info;
  GetSystemInfo(&sys_info);
  return sys_info.dwNumberOfProcessors;
#else
  return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

