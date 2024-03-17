#include "os_timer_internal.h"
#include "os_io_external.h"
#include "os_def.h"
#include "os_task_external.h"
#include "os_sched_external.h"
#include "os_debug_external.h"
#include "os_hwi_external.h"

volatile U32 g_sysTicks = 0;
/* 时间片ticks */
U32 g_timeSliceTicks;

INLINE void OsTimerSetFreq(U8 counterPort, U8 counterNum, U8 rwl,
                           U8 counterMode, U16 counterVal)
{
  OsOutB(PIT_CONTROL_PORT, OS_TIMER_BUILD_FREQ_PORT_MODE(counterNum, rwl, counterMode));
  OsOutB(counterPort, (U8)counterVal);
  OsOutB(counterPort, (U8)(counterVal >> 8));
}

static void OsTimerHwiHandler(void)
{
  struct OsTaskCb *curTask = OS_RUNNING_TASK();

  curTask->ticks++;
  curTask->delayTicks--;

  if (curTask->ticks == g_timeSliceTicks) {
    curTask->status = OS_TASK_TIME_SLICE_PENDING;
    /* 从ready队列里删掉 */
    OsSchedDelTskFromRdyList(curTask);
    /* 加入到ready队列尾部 */
    OsSchedAddTskToRdyListTail(curTask);
  }
}

void OsTimerInit(void)
{
  OS_DEBUG_PRINT_STR("OsTimerInit start.\n");
  OsTimerSetFreq(COUNTER0_PORT, COUNTER0_NO, READ_WRITE_LATCH, COUNTER_MODE, COUNTER0_VALUE);
  (void)OsHwiCreate(0x20, OsTimerHwiHandler, NULL);
  OS_DEBUG_PRINT_STR("OsTimerInit OK.\n");
}
