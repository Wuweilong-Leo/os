#ifndef OS_SCHED_INTERNAL_H
#define OS_SCHED_INTERNAL_H
#include "os_btmp_external.h"
#include "os_def.h"
#include "os_sched_external.h"
#include "os_task_external.h"
#define OS_GET_READY_LIST(prio) (&g_runQue.readyList[prio])
#endif