#ifndef OS_CPU_I386_H
#define OS_CPU_I386_H
#include "os_def.h"
#include "os_task_external.h"
#include "string.h"

#define OS_FAST_SAVE_FLAG 0x0UL
#define OS_ALL_SAVE_FLAG 0x1UL

struct OsFastSaveStack {
  U32 saveFlag;
  U32 ebp;
  U32 ebx;
  U32 edi;
  U32 esi;
  taskEntry eip; /* 汇编代码ret后，会回到此函数 */
  void *rsvd;    /* 必须保留，充当返回地址圧栈空间 */
  void *arg[OS_TASK_ARG_NUM];
};

INLINE uintptr_t OsSetTaskStartStack(uintptr_t stkBot, taskEntry entry,
                                     void **arg) {
  struct OsFastSaveStack *setMemBase;

  setMemBase =
      (struct OsFastSaveStack *)((U32)stkBot - sizeof(struct OsFastSaveStack));

  setMemBase->saveFlag = OS_FAST_SAVE_FLAG;
  setMemBase->ebp = 0;
  setMemBase->ebx = 0;
  setMemBase->edi = 0;
  setMemBase->esi = 0;
  setMemBase->eip = entry;
  memcpy(setMemBase->arg, arg, sizeof(void *) * OS_TASK_ARG_NUM);

  return (uintptr_t)setMemBase;
}
#endif