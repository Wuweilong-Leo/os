#ifndef OS_HWI_EXTERNAL_H
#define OS_HWI_EXTERNAL_H
#include "os_def.h"
#if (OS_CPU_TYPE == OS_I386)
#include "os_i386_hwi.h"
#endif
typedef void (*intHandler) (void *arg);

enum OsIntStatus { INT_OFF, INT_ON };

extern U32 OsHwiCreate(U32 hwiNum, intHandler handler, void *arg);

INLINE enum OsIntStatus OsIntLock(void) { return OsHwiDisable(); }

INLINE enum OsIntStatus OsIntUnlock(void) { return OsHwiEnable(); }

INLINE void OsIntRestore(enum OsIntStatus status) { OsHwiSetStatus(status); }
#endif