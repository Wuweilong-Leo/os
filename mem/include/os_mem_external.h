#ifndef OS_MEM_EXTERNAL_H
#define OS_MEM_EXTERNAL_H
#include "os_def.h"
#define OS_PG_SIZE 4096

extern uintptr_t OsMemAllocPgs(U32 pgNum);
extern U32 OsMemAllocPgsByAddr(uintptr_t vaddr);
extern U32 OsMemFree(void *addr);
#endif