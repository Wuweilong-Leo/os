#include "os_def.h"
#include "os_mem_internal.h"
#include "string.h"

struct OsPool g_phyMemPool;
struct OsPool g_virMemPool;

INLINE U32 OsMemGetBtmpSizeByPgNum(U32 pgNum) {
  return (pgNum % 8 == 0) ? (pgNum / 8) : (pgNum / 8 + 1);
}

INLINE U32 OsMemInitPool(struct OsMemPool *pool, U8 *btmpBase, void *base,
                         U32 pgNum) {
  struct OsMemPool *memPool = pool;
  struct OsBtmp *usedPgMask = &memPool->usedPgMask;

  OsBtmpInit(usedPgMask, btmpBase, pgNum);
  memPool->base = base;
  memPool->size = pgNum * OS_PG_SIZE;
}

#define OS_PG_TAB_SIZE (OS_PG_SIZE * (OS_PG_DIR_CNT + OS_PG_TAB_CNT))
#define OS_USED_MEM_SIZE (OS_PG_TAB_SIZE + OS_PG_DIR_START_ADDR)
#define OS_FREE_MEM_BASE OS_USED_MEM_SIZE
#define OS_GET_FREE_MEM_SIZE(allMemSize) ((allMemSize) - OS_USED_MEM_SIZE)
#define OS_GET_FREE_PG_NUM(freeMemSize) ((freeMemSize) / OS_PG_SIZE)

INLINE void OsMemConfigPool(U32 allMemSize)
{
  U32 freeMemSize;
  U32 allFreePgNum;
  U32 phyBtmpLen;
  uintptr_t phyMemBase;

  freeMemSize = OS_GET_FREE_MEM_SIZE(allMemSize);
  allFreePgNum = OS_GET_FREE_PG_NUM(freeMemSize);
  phyBtmpLen = OsMemGetBtmpSizeByPgNum(allFreePgNum);

  phyMemBase = (uintptr_t)OS_FREE_MEM_BASE;

  OsMemInitPool(&g_phyMemPool, (U8 *)OS_MEM_BITMAP_BASE, phyMemBase, allFreePgNum);
  OsMemInitPool(&g_virMemPool, (U8 *)(OS_MEM_BITMAP_BASE + phyBtmpLen),
                (uintptr_t)OS_VIR_MEM_START_ADDR, allFreePgNum);
}