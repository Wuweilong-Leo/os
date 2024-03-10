#ifndef OS_MEM_INTERNAL_H
#define OS_MEM_INTERNAL_H
#include "os_btmp_external.h"
#include "os_def.h"
#include "os_mem_external.h"
#include "os_sem_external.h"

/* 内存池 */
struct OsMemPool {
  /* 使用了的页位图 */
  struct OsBtmp usedPgMask;
  /* 内存池管理内存的基地址 */
  void *base;
  /* 内存池管理内存的字节大小 */
  U32 size;
};

#define OS_MEM_BITMAP_BASE 0XC009A000
#define OS_VIR_MEM_START_ADDR 0XC0100000

#define OS_PG_DIR_START_ADDR 0X100000
#define OS_PG_DIR_CNT 1
#define OS_PG_TAB_CNT 255
#endif