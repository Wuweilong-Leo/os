#include "os_hwi_internal.h"
#include "os_sched_external.h"

struct OsHwiForm g_hwiForm[OS_HWI_MAX_NUM] = {0};

U32 OsHwiCreate(U32 hwiNum, intHandler handler, void *arg) {
  g_hwiForm[hwiNum].handler = handler;
  g_hwiForm[hwiNum].arg = arg;
  return OS_OK;
}

U32 OsHwiTail(void) {   
  OsSchedMainProc(); 
}