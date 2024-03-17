#ifndef OS_SCHED_EXTERNAL_H
#define OS_SCHED_EXTERNAL_H
#include "os_btmp_external.h"
#include "os_task_external.h"

#define OS_PRIO_MAX_NUM 8
#define OS_TASK_MAX_NUM 32
struct OsRunQue {
  /* 正在运行的任务 */
  struct OsTaskCb *curTask;
  U32 uniFlag;
  bool needSched;
  struct OsTaskCb *idleTask;
  struct OsBtmp readyListMask;
  struct OsList readyList[OS_PRIO_MAX_NUM];
  struct OsList delayList;
};

extern struct OsRunQue g_runQue;
extern void OsSchedMainProc(void);
extern void OsSchedDelTskFromRdyList(struct OsTaskCb *tsk);
extern void OsSchedAddTskToRdyListTail(struct OsTaskCb *tsk);
extern void OsSchedActive(void);

#define OS_RUNNING_TASK() (g_runQue.curTask)

#endif