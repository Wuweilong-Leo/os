#ifndef OS_SEM_EXTERNAL_H
#define OS_SEM_EXTERNAL_H
#include "os_def.h"
#include "os_list_external.h"
#include "os_task_external.h"

// 信号量
struct OsSem {
  U32 curCount;
  U32 totalCount;
  struct OsTaskCb *holder;
  struct OsList pendList;
  /* 如果某个任务持有此信号量，挂载在OsTaskCb的semList上 */
  struct OsList semListNode;
};

extern void OsSemInit(struct OsSem *sem, U8 cnt);
extern U32 OsSemPend(struct OsSem *sem);
extern U32 OsSemPost(struct OsSem *sem);
#endif