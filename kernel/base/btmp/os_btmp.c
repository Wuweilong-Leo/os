#include "os_btmp_external.h"

INLINE U32 OsBtmpGetByteLenByBitNum(U32 bitNum) {
  return (bitNum % 8) ? (bitNum / 8 + 1) : (bitNum / 8);
}

void OsBtmpInit(struct OsBtmp *btmp, U8 *base, U32 bitNum) {
  btmp->base = base;
  btmp->bitNum = bitNum;
  btmp->byteLen = OsBtmpGetByteLenByBitNum(bitNum);
  memset(btmp->base, 0, btmp->byteLen);
}

U32 OsBtmpGet(struct OsBtmp *btmp, U32 bitIdx) {
  U32 byteOff = bitIdx / 8;
  U32 bitOff = bitIdx % 8;
  return (btmp->base[byteOff] & (1 << bitOff)) != 0;
}

/* 连续申请cnt个为val的位, val只能为1或者0 */
bool OsBtmpScan(struct OsBtmp *btmp, U32 val, U32 cnt, U32 *idx) {
  U32 left = 0;
  U32 right = 0;
  while (right < btmp->bitNum) {
    if (OsbtmpGet(btmp, right) != val) {
      left = right + 1;
    }
    if (right - left + 1 == cnt) {
      *idx = left;
      return TRUE;
    }
    right++;
  }
  return FALSE;
}

void OsBtmpSet(struct OsBtmp *btmp, U32 idx) {
  U32 byteOff = idx / 8;
  U32 bitOff = idx % 8;
  btmp->base[byteOff] |= (1 << bitOff);
}

void OsBtmpClear(struct OsBtmp *btmp, U32 idx) {
  U32 byteOff = idx / 8;
  U32 bitOff = idx % 8;
  btmp->base[byteOff] &= ~(1 << bitOff);
}