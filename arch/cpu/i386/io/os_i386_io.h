#ifndef OS_I386_IO_H
#define OS_I386_IO_H
#include "os_def.h"

INLINE void OsOutB(U16 port, U8 data) {
  OS_EMBED_ASM("outb %b0, %w1" ::"a"(data), "Nd"(port));
}
INLINE void OsOutSw(U16 port, const void *addr, U32 wordCnt) {
  OS_EMBED_ASM("cld; rep outsw" : "+S"(addr), "+c"(wordCnt) : "d"(port));
}
INLINE U8 OsInB(U16 port) {
  U8 data;
  OS_EMBED_ASM("inb %w1, %b0" : "=a"(data) : "Nd"(port));
  return data;
}
INLINE void OsInSw(U16 port, void *addr, U32 wordCnt) {
  OS_EMBED_ASM("cld; rep insw"
               : "+D"(addr), "+c"(wordCnt)
               : "d"(port)
               : "memory");
}
#endif