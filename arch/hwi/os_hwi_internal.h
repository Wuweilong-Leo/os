#ifndef OS_HWI_INTERNAL_H
#define OS_HWI_INTERNAL_H
#include "os_builddef.h"
#if (OS_CPU_TYPE == OS_I386)
#include "os_i386_hwi.h"
#endif

struct OsHwiForm {
  intHandler handler;
  void *arg;
};

#endif