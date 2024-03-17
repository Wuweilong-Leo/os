#include "os_hwi_internal.h"
#include "os_sched_external.h"

struct OsHwiForm g_hwiForm[OS_HWI_MAX_NUM] = {0};

U32 OsHwiCreate(U32 hwiNum, intHandler handler, void *arg) {
  g_hwiForm[hwiNum].handler = handler;
  g_hwiForm[hwiNum].arg = arg;
  return OS_OK;
}

void OsHwiHandle(U32 hwiNum) {
  intHandler handler = g_hwiForm[hwiNum].handler;
  void *arg = g_hwiForm[hwiNum].arg;

  handler(arg);
}

INLINE void OsHwiProcDelayTask(void) {
  struct OsList *delayList = &g_runQue.delayList;
  struct OsTaskCb *delayTask;

  while (delayList->next != delayList) {
    delayTask = OS_LIST_GET_STRUCT_ENTRY(struct OsTaskCb, delayListNode, delayList->next);
    if (delayTask->delayTicks == 0) {
      OsSchedAddTskToRdyListTail(delayTask);
    }
    delayList = delayList->next;
  }
  
}

U32 OsHwiTail(void) {
  OsHwiProcDelayTask();   
  OsSchedMainProc(); 
}