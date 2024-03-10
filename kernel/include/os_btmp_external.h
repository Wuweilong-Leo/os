#ifndef OS_BTMP_EXTENAL_H
#define OS_BTMP_EXTENAL_H
#include "os_def.h"

struct OsBtmp {
  /* 位图基地址 */
  U8 *base;
  /* 位图的位数 */
  U32 bitNum;
  /* 位图的字节数 */
  U32 byteLen;
};

void OsBtmpInit(struct OsBtmp *btmp, U8 *base, U32 bitNum);
U32 OsBtmpGet(struct OsBtmp *btmp, U32 bitIdx);
bool OsBtmpScan(struct OsBtmp *btmp, U32 val, U32 cnt, U32 *idx);
void OsBtmpSet(struct OsBtmp *btmp, U32 idx);
void OsBtmpClear(struct OsBtmp *btmp, U32 idx);
#endif