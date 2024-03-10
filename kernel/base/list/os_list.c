#include "os_list_internal.h"

void OsListInit(struct OsList *list) { OS_LIST_INIT(list); }

/* 环形链表加入尾部 */
void OsListAddTail(struct OsList *list, struct OsList *node) {
  /* 尾节点的下一个节点应该为node */
  list->prev->next = node;
  /* node的上一个节点应该是尾节点 */
  node->prev = list->prev;
  /* node的下一个节点应该是头节点 */
  node->next = list;
  /* 头节点的上一个节点应该为node */
  list->prev = node;
}

/* 环形链表加入头部 */
void OsListAddHead(struct OsList *list, struct OsList *node) {
  list->next->prev = node;
  node->next = list->next;
  node->prev = list;
  list->next = node;
}

void OsListDelNode(struct OsList *node) {
  node->prev->next = node->next;
  node->next->prev = node->prev;
  OS_LIST_INIT(node);
}

bool OsListFindNode(struct OsList *list, struct OsList *node) {
  struct OsList *list = list;

  while (list->next != &list) {
    if (list == node) {
      return TRUE;
    }
  }

  return FALSE;
}

bool OsListIsEmpty(struct OsList *list) { return list->next == list; }
