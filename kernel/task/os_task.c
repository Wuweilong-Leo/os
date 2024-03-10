#include "os_cpu_external.h"
#include "os_hwi_external.h"
#include "os_list_external.h"
#include "os_mem_external.h"
#include "os_sched_external.h"
#include "os_task_internal.h"
#include "string.h"

struct OsTaskCb g_taskCbArray[OS_TASK_MAX_NUM];
struct OsList g_taskFreeList;

/* Task模块初始化 */
U32 OsTaskConfig(void) {
  U32 i;

  OsListInit(&g_taskFreeList);

  for (i = 0; i < OS_TASK_MAX_NUM; i++) {
    g_taskCbArray[i].pid = i;
    g_taskCbArray[i].status = OS_TASK_NOT_CREATE;
    OsListInit(&g_taskCbArray[i].freeListNode);
    OsListAddTail(&g_taskFreeList, &g_taskCbArray[i].freeListNode);
  }

  return OS_OK;
}

INLINE U32 OsTaskAllocStack(uintptr_t *stackPtr) {
  uintptr_t stkMemBase;
  uintptr_t stkBot;
  uintptr_t stkPtr;
  U32 i;

  /* 申请4K内存作为栈 */
  stkMemBase = OsMemAlloc(OS_PG_SIZE);
  if (stackPtr == NULL) {
    return OS_TASK_CREATE_ALLOC_STACK_NO_MEM;
  }

  /* 把栈初始化成魔术字 */
  for (i = 0; i < OS_PG_SIZE; i += 2) {
    *((U8 *)stkMemBase + i) = 0xcc;
    *((U8 *)stkMemBase + i + 1) = 0xaa;
  }

  stkBot = (uintptr_t)((U32)stkMemBase + OS_PG_SIZE);

  *stackPtr = stkBot;

  return OS_OK;
}

U32 OsTaskCreate(struct OsTaskCreateParam *param, U32 *taskId) {
  U32 ret;
  U32 taskId;
  struct OsTaskCb *freeCb;
  uintptr_t stkBot;
  uintptr_t stkPtr;
  enum OsIntStatus intSave;

  intSave = OsIntLock();
  if (OsListIsEmpty(&g_taskFreeList)) {
    return OS_TASK_ERR_NO_FREE_CB;
  }

  freeCb = OS_LIST_GET_STRUCT_ENTRY(struct OsTaskCb, freeListNode,
                                    g_taskFreeList.next);
  OsListDelNode(&freeCb->freeListNode);
  OsIntRestore(intSave);

  memcpy(freeCb->name, param->name, OS_TASK_NAME_MAX_SIZE);
  freeCb->prio = param->prio;
  freeCb->status = OS_TASK_NOT_RESUME;
  freeCb->entry = param->entryFunc;
  memcpy(freeCb->arg, param->arg, sizeof(void *) * OS_TASK_ARG_NUM);

  ret = OsTaskAllocStack(&stkBot);
  if (ret != OS_OK) {
    return ret;
  }

  stkPtr = OsSetTaskStartStack(stkBot, freeCb->entry, freeCb->arg);
  freeCb->stkPtr = stkPtr;

  *taskId = freeCb->pid;

  return OS_OK;
}

U32 OsTaskResume(U32 taskId) {
  struct OsTaskCb *taskCb = &g_taskCbArray[taskId];

  /* 必须创建了并没resume才能resume */
  if (taskCb->status != OS_TASK_NOT_RESUME) {
    return OS_TASK_ERR_RESUME_STATUS_WRONG;
  }

  taskCb->status = OS_TASK_READY;

  /* 加入readyList, 参与调度 */
  OsSchedAddTskToReadyList(taskCb);

  return OS_OK;
}