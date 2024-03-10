#ifndef OS_PRINT_EXTERNAL_H
#define OS_PRINT_EXTERNAL_H
#include "os_def.h"
typedef void (*OsPrintCharHook)(char);
extern void OsPrintStr(char *str);
extern void OsPrintHex(U32 num);
#endif