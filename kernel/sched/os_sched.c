#include "os_btmp_external.h"
#include "os_sched_internal.h"

struct OsRunQue g_runQue;

void OsSchedActive(void) {
  if (!g_runQue.needSched) {
    return;
  }
  /* 上下文切换*/
  OsTaskTrap(OS_RUNNING_TASK());
  return;
}

/* 选取最高优先级队列里的第一个任务 */
struct OsTaskCb *OsSchedPickNextTask(void) {
  U32 ret;
  U32 highestPrio;
  struct OsTaskCb *nextTask;
  struct OsList *readyList;

  /* 不可能一个任务也没有 */
  (void)OsBtmpScan(&g_runQue.readyListMask, 1, 1, &highestPrio);

  readyList = &g_runQue.readyList[highestPrio];
  nextTask =
      OS_LIST_GET_STRUCT_ENTRY(struct OsTaskCb, readyListNode, readyList->next);

  return nextTask;
}

void OsSchedDelTskFromReadyList(struct OsTaskCb *tsk) {
  struct OsList *readyList;
  struct OsBtmp *readyListMask;
  U32 prio = tsk->prio;

  readyList = OS_GET_READY_LIST(prio);
  readyListMask = &g_runQue.readyListMask;

  OsListDelNode(&tsk->readyListNode);

  if (OsListIsEmpty(readyList)) {
    OsBtmpClear(readyListMask, prio);
  }

  if (tsk == OS_RUNNING_TASK()) {
    g_runQue.needSched = TRUE;
  }
}

void OsSchedAddTskToReadyList(struct OsTaskCb *tsk) {
  struct OsList *readyList;
  struct OsBtmp *readyListMask;
  U32 prio = tsk->prio;
  struct OsTaskCb *curTask = OS_RUNNING_TASK();

  readyList = OS_GET_READY_LIST(prio);
  readyListMask = &g_runQue.readyListMask;

  OsListAddTail(readyList, &tsk->readyListNode);
  OsBtmpSet(readyListMask, prio);

  if (prio < curTask->prio) {
    g_runQue.needSched = TRUE;
  }
}

void OsSchedMainProc(void) {
  struct OsTaskCb *curTask = OS_RUNNING_TASK();
  struct OsTaskCb *nextTask = curTask;

  if (g_runQue.needSched) {

    nextTask = OsSchedPickNextTask();
    nextTask->status = OS_TASK_RUNNING;
    g_runQue.curTask = nextTask;
  }

  /* 如果没有就绪的任务，直接返回到原任务执行 */
  OsContextLoad(nextTask);
}