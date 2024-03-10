#ifndef OS_TASK_INTERNAL_H
#define OS_TASK_INTERNAL_H
#include "os_def.h"
#include "os_sys.h"
#include "os_task_external.h"

#define OS_TASK_ERR_NO_FREE_CB OS_BUILD_ERR_CODE(OS_TASK_MID, 0x0)
#define OS_TASK_ERR_RESUME_STATUS_WRONG OS_BUILD_ERR_CODE(OS_TASK_MID, 0x1)
#define OS_TASK_CREATE_ALLOC_STACK_NO_MEM OS_BUILD_ERR_CODE(OS_TASK_MID, 0x2)
#endif