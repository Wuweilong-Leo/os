#ifndef OS_DEBUG_EXTERNAL_H
#define OS_DEBUG_EXTERNAL_H
#include "os_def.h"

#define DEBUG_ENABLE TRUE

extern void OsDebugPanicSpin(char *filename, int line, const char *func,
                             const char *condition);
#define OS_PANIC(...)                                                          \
  OsDebugPanicSpin(__FILE__, __LINE__, __func__, __VA_ARGS__)

#if (DEBUG_ENABLE == TRUE)
#define OS_ASSERT(condition)                                                   \
  do {                                                                         \
    if (condition) {                                                           \
      ;                                                                        \
    } else {                                                                   \
      OS_PANIC(#condition);                                                    \
    }                                                                          \
  } while (0);
#else
#define ASSERT(condition)
#endif

#endif