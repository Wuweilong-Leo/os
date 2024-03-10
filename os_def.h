#ifndef OS_DEF_H
#define OS_DEF_H
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef char S8;
typedef short S16;
typedef int S32;
typedef U8 bool;
typedef void *uintptr_t;
#define TRUE 1
#define FALSE 0
#define NULL ((void *)0)
#define OS_OK 0

/* 强制内联 */
#define INLINE static __attribute__((always_inline))

#define OS_EMBED_ASM(asmCode) __asm volatile(asmCode)

#define OS_BUILD_ERR_CODE(mid, errCode) (((mid) << 16) | ((0xFFFF) & (errCode)))
#endif