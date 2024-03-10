#ifndef OS_I386_HWI_H
#define OS_I386_HWI_H
#include "os_def.h"
#include "os_hwi_external.h"
#define OS_HWI_MAX_NUM 0x81
#define OS_EFLAGS_IF_MASK 0x200
#define PIC_M_CTRL 0x20
#define PIC_M_DATA 0x21
#define PIC_S_CTRL 0xa0
#define PIC_S_DATA 0xa1

INLINE U32 OsHwiGetEflags(void) {
  U32 eflags;
  OS_EMBED_ASM("pushf; popl %0" : "=g"(eflags));
  return eflags;
}

INLINE enum OsIntStatus OsHwiGetIntStatus(void) {
  U32 eflags;
  eflags = OsHwiGetEflags();
  return (eflags & OS_EFLAGS_IF_MASK) ? INT_ON : INT_OFF;
}

INLINE enum OsIntStatus OsHwiDisable(void) {
  if (OsHwiGetIntStatus() == INT_ON) {
    OS_EMBED_ASM("cli" ::: "memory");
    return INT_ON;
  }
  return INT_OFF;
}

INLINE enum OsIntStatus OsHwiEnable(void) {
  if (OsHwiGetIntStatus() == INT_OFF) {
    OS_EMBED_ASM("sti");
    return INT_OFF;
  }
  return INT_ON;
}

INLINE void OsHwiSetStatus(enum OsIntStatus status) {
  if (status == INT_ON) {
    (void)OsHwiEnable();
  } else {
    (void)OsHwiDisable();
  }
}

#endif