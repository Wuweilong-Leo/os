#ifndef OS_MEM_EXTERNAL_H
#define OS_MEM_EXTERNAL_H
#include "os_def.h"
#define OS_PG_SIZE 4096

extern uintptr_t OsMemAllocPgs(U32 size);
extern U32 OsMemFree(void *addr);
#endif