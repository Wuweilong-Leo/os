#ifndef OS_LIST_INTERNAL_H
#define OS_LIST_INTERNAL_H
#include "os_list_external.h"
#define OS_LIST_INIT(list)                                                     \
  do {                                                                         \
    list->next = list;                                                         \
    list->prev = list;                                                         \
  } while (0);
#endif