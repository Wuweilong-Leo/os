#ifndef OS_LIST_EXTERNAL_H
#define OS_LIST_EXTERNAL_H
#include "os_def.h"

struct OsList {
  struct OsList *prev;
  struct OsList *next;
};

/* 获取结构体内元素的偏移 */
#define OFFSET(structType, elem) ((U32)(&(((structType *)0)->elem)))

/* 通过元素地址获取结构体的首地址 */
#define OS_LIST_GET_STRUCT_ENTRY(structType, elemName, elemAddr)               \
  ((structType *)((U32)elemAddr - OFFSET(structType, elemName)))

extern void OsListInit(struct OsList *list);
extern void OsListAddTail(struct OsList *list, struct OsList *node);
extern void OsListAddHead(struct OsList *list, struct OsList *node);
extern void OsListDelNode(struct OsList *node);
extern bool OsListIsEmpty(struct OsList *list);
extern bool OsListFindNode(struct OsList *list, struct OsList *node);
#endif