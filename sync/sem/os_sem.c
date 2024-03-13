#include "os_hwi_external.h"
#include "os_list_external.h"
#include "os_sched_external.h"
#include "os_sem_internal.h"
#include "os_task_external.h"

void OsSemInit(struct OsSem *sem, U8 cnt) {
  sem->totalCount = cnt;
  sem->curCount = cnt;
  sem->holder = NULL;
  OsListInit(&sem->pendList);
  OsListInit(&sem->semListNode);
}

/* 尝试获取信号量 */
U32 OsSemPend(struct OsSem *sem) {
  struct OsList *pendList = &sem->pendList;
  struct OsList *semListNode = &sem->semListNode;
  struct OsTaskCb *curTask = OS_RUNNING_TASK();
  struct OsList *tskPendListNode = &curTask->pendListNode;
  enum OsIntStatus intSave;

  intSave = OsIntLock();

  /* 如果此时信号量持有者是本身，不允许再持有 */
  if (sem->holder == curTask) {
    return OS_SEM_ERR_PEND_AGAIN;
  }

  while (sem->curCount == 0) {
    /* 获取不到信号量 */
    if (!OsListFindNode(pendList, tskPendListNode)) {
      OsListAddTail(pendList, tskPendListNode);
    }

    OsSchedDelTskFromRdyList(curTask);
    curTask->status = OS_TASK_SEM_PENDING;
    /* 进行任务调度 */
    OsSchedActive();
  }

  sem->curCount--;
  sem->holder = curTask;
  OsListAddTail(&curTask->semList, semListNode);

  OsIntRestore(intSave);

  return OS_OK;
}

/* 释放信号量 */
U32 OsSemPost(struct OsSem *sem) {
  struct OsList *pendList = &sem->pendList;
  struct OsList *semListNode = &sem->semListNode;
  struct OsTaskCb *curTask = OS_RUNNING_TASK();
  struct OsTaskCb *nextTask;
  enum OsIntStatus intSave;

  intSave = OsIntLock();

  if (sem->holder != curTask) {
    return OS_SEM_ERR_POST_NOT_HOLDER;
  }

  if (sem->curCount == sem->totalCount) {
    return OS_SEM_ERR_POST_NO_PEND;
  }

  sem->curCount++;
  sem->holder = NULL;
  OsListDelNode(semListNode);

  nextTask =
      OS_LIST_GET_STRUCT_ENTRY(struct OsTaskCb, pendListNode, pendList->next);
  OsListDelNode(&nextTask->pendListNode);
  OsSchedAddTskToRdyListTail(nextTask);
  nextTask->status = OS_TASK_READY;

  OsIntRestore(intSave);

  return OS_OK;
}
