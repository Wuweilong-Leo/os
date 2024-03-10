#ifndef OS_MEM_EXTERNAL_H
#define OS_MEM_EXTERNAL_H
#define OS_PG_SIZE 4096

extern void *OsMemAlloc(U32 size);
extern U32 OsMemFree(void *addr);
#endif