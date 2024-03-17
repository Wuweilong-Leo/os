#include "os_debug_external.h"
#include "os_def.h"
#include "os_mem_internal.h"
#include "string.h"

U32 g_totalMemSize;
struct OsMemPool g_phyMemPool;
struct OsMemPool g_virMemPool;

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

INLINE void OsMemConfigPool(U32 allMemSize) {
  U32 freeMemSize;
  U32 allFreePgNum;
  U32 phyBtmpLen;
  uintptr_t phyMemBase;

  freeMemSize = OS_GET_FREE_MEM_SIZE(allMemSize);
  allFreePgNum = OS_GET_FREE_PG_NUM(freeMemSize);
  phyBtmpLen = OsMemGetBtmpSizeByPgNum(allFreePgNum);

  phyMemBase = (uintptr_t)OS_FREE_MEM_BASE;

  OsMemInitPool(&g_phyMemPool, (U8 *)OS_MEM_BITMAP_BASE, phyMemBase,
                allFreePgNum);
  OsMemInitPool(&g_virMemPool, (U8 *)(OS_MEM_BITMAP_BASE + phyBtmpLen),
                (uintptr_t)OS_VIR_MEM_START_ADDR, allFreePgNum);
}

void OsMemConfig(void) {
  OS_DEBUG_PRINT_STR("OsMemConfig start\n");
  OsMemConfigPool(g_totalMemSize);
  OS_DEBUG_PRINT_STR("OsMemConfig end\n");
}

/* 获取空闲内存页地址基地址 */
INLINE uintptr_t OsMemGetFreePgsAddr(struct OsMemPool *memPool, U32 pgNum) {
  U32 pgIdx;
  U32 pgNum;
  U32 addrBase;
  U32 i;

  if (!OsBtmpScan(&memPool->usedPgMask, 0, pgNum, &pgIdx)) {
    OS_DEBUG_PRINT_STR("OsMemGetVaddr: OsBtmpScan failed\n");
    return NULL;
  }

  for (i = pgIdx; i < pgIdx + pgNum; i++) {
    OsBtmpSet(&memPool->usedPgMask, i);
  }

  addrBase = (uintptr_t)((U32)memPool->base + OS_PG_SIZE * pgIdx);

  return addrBase;
}

/* 根据虚拟地址找此虚拟地址对应的页表项的虚拟地址 */
INLINE uintptr_t OsMemGetPteVirAddr(uintptr_t vaddr) {
  uintptr_t pte;
  pte = (uintptr_t)(0xFFC00000 + (((U32)vaddr & 0xFFC00000) >> 10) +
                    OS_PTE_IDX((U32)vaddr) * OS_PTE_SIZE);
  return pte;
}

/* 根据虚拟地址找此虚拟地址对应的页目录项的虚拟地址 */
INLINE uintptr_t OsMemGetPdeVirAddr(uintptr_t vaddr) {
  uintptr_t pde;
  pde = (uintptr_t)(0xFFFFF000 + OS_PDE_IDX((U32)vaddr) * OS_PDE_SIZE);
  return pde;
}

void OsMemMapVir2Phy(uintptr_t vaddr, uintptr_t paddr) {
  U32 *pteVaddr;
  U32 *pdeVaddr;
  uintptr_t ptPhyAddr;

  pteVaddr = (U32 *)OsMemGetPteVirAddr(vaddr);
  pdeVaddr = (U32 *)OsMemGetPdeVirAddr(vaddr);

  /* 如果页目录项已存在，则对应页表已经存在，只用更改页表项 */
  if (OS_PDE_EXIST(pdeVaddr)) {
    /* 如果页表项还不存在，添加页表项 */
    if (!OS_PTE_EXIST(pteVaddr)) {
      *pteVaddr = (U32)paddr | OS_PG_US_U | OS_PG_RW_W | OS_PG_P_1;
    } else {
      OS_DEBUG_PRINT_STR("pte repeat\n");
      *pteVaddr = (U32)paddr | OS_PG_US_U | OS_PG_RW_W | OS_PG_P_1;
    }
  } else {
    /* 如果页目录项不存在，说明没对应页表，先申请4K物理内存作为页表 */
    ptPhyAddr = OsMemGetFreePgsAddr(&g_phyMemPool, 1);
    /* 把页表物理地址写入页目录, 一旦写入，可以通过pte来访问页表项了。*/
    *pdeVaddr = (U32)ptPhyAddr | OS_PG_US_U | OS_PG_RW_W | OS_PG_P_1;
    /* 把整张页表初始化为0 */
    memset((uintptr_t)((U32)pteVaddr & 0xFFFFF000), 0, OS_PG_SIZE);
    /* 写入页表项 */
    *pteVaddr = (U32)paddr | OS_PG_US_U | OS_PG_RW_W | OS_PG_P_1;
  }
}

/* 0 < pgNum < 30 * 1024 */
uintptr_t OsMemAllocPgs(U32 pgNum) {
  uintptr_t vaddr;
  uintptr_t vaddrBase;
  uintptr_t paddr;
  U32 i;

  vaddrBase = OsMemGetFreePgsAddr(&g_virMemPool, pgNum);
  if (vaddrBase == NULL) {
    return NULL;
  }

  vaddr = vaddrBase;
  for (i = 0; i < pgNum; i++) {
    paddr = OsMemGetFreePgsAddr(&g_phyMemPool, 1);
    if (paddr == NULL) {
      return NULL;
    }

    /* 虚实映射 */
    OsMemMapVir2Phy(vaddr, paddr);

    vaddr = (uintptr_t)((U32)vaddr + OS_PG_SIZE);
  }

  return vaddrBase;
}

/* vaddr必须4096对齐 */
U32 OsMemAllocPgsByAddr(uintptr_t vaddr) {
  U32 idx;
  uintptr_t paddr;

  if (!OS_ADDR_PG_ALIGN((U32)vaddr)) {
    return OS_MEM_ERR_VADDR_NOT_PG_ALIGN;
  }

  idx = ((U32)vaddr - (U32)g_virMemPool.base) / OS_PG_SIZE;

  if (OsBtmpGet(&g_virMemPool.usedPgMask, idx) == 1) {
    return OS_MEM_ERR_VIR_PG_NOT_FREE;
  }

  OsBtmpSet(&g_virMemPool.usedPgMask, idx);

  if (!OsBtmpScan(&g_phyMemPool.usedPgMask, 0, 1, &idx)) {
    return OS_MEM_ERR_NO_FREE_PHY_PG;
  }

  OsBtmpSet(&g_phyMemPool.usedPgMask, idx);
  paddr = (uintptr_t)((U32)g_phyMemPool.base + idx * OS_PG_SIZE);

  OsMemMapVir2Phy(vaddr, paddr);

  return OS_OK;
}