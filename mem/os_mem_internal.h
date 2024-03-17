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

#define OS_MEM_BITMAP_BASE 0xC009A000U
#define OS_VIR_MEM_START_ADDR 0xC0100000U

#define OS_PG_DIR_START_ADDR 0x100000U
#define OS_PG_DIR_CNT 1U
#define OS_PG_TAB_CNT 255U

/* 虚拟地址在对应的页目录的索引 */
#define OS_PDE_IDX(addr) (((addr) & 0xFFC00000U) >> 22)
/* 虚拟地址在对应的页表的索引 */
#define OS_PTE_IDX(addr) (((addr) & 0x003FF000U) >> 12)

/* 页目录项和页表项大小都是4B*/
#define OS_PTE_SIZE 4U
#define OS_PDE_SIZE 4U

#define OS_PTE_EXIST(pteVaddr) (((*(pteVaddr)) & 0x1) != 0)
#define OS_PDE_EXIST(pdeVaddr) (((*(pdeVaddr)) & 0x1) != 0)

#define OS_PG_P_1 1U
#define OS_PG_P_0 0U
#define OS_PG_RW_R 0U
#define OS_PG_RW_W 2U
#define OS_PG_US_S 0U
#define OS_PG_US_U 4U

#define OS_PG_TAB_SIZE (OS_PG_SIZE * (OS_PG_DIR_CNT + OS_PG_TAB_CNT))
#define OS_USED_MEM_SIZE (OS_PG_TAB_SIZE + OS_PG_DIR_START_ADDR)
#define OS_FREE_MEM_BASE OS_USED_MEM_SIZE
#define OS_GET_FREE_MEM_SIZE(allMemSize) ((allMemSize)-OS_USED_MEM_SIZE)
#define OS_GET_FREE_PG_NUM(freeMemSize) ((freeMemSize) / OS_PG_SIZE)

#endif