#include "os_cpu_i386.h"

[bits 32]

section .text

global OsTaskTrap

OsTaskTrap:

    ;前后线程的esi,edi,ebx,ebp 不同，因此要保证先存起来，再出栈。
    push esi
    push edi
    push ebx
    push ebp
    push OS_FAST_SAVE_FLAG

    mov eax, [esp + 24] ;eax 里存放 当前线程tcb首地址
    mov [eax], esp ; 把当前esp地址存入tcb中

    call OsSchedMainProc

global OsContextLoad

OsContextLoad:
    mov eax, [esp + 4];eax 里存放 将执行线程tcb首地址
    mov esp, [eax] ; 把下一个线程的栈地址写入esp,完成栈的切换

    pop eax
    cmp eax, OS_ALL_SAVE_FLAG
    je OsAllLoad

OsFastLoad:

    pop ebp
    pop ebx
    pop edi
    pop esi

    ret

OsAllLoad:

    popad
    pop gs
    pop fs
    pop es
    pop ds
    
    ; skip error code
    add esp, 4 

    iret

%define OS_ERROR_CODE nop
%define ZERO push 0

extern g_runQue

%macro OS_OS_VECTOR 2
section .text
OsHwiOS_VECTOR%1:
    %2 // 压入错误码或者0
    // 压入上下文
    push ds
    push es
    push fs
    push gs
    pushad
    push OS_ALL_SAVE_FLAG

    mov eax, [g_runQue]
    mov [eax], esp

    // 清中断 
    mov al, 0x20
    out 0xa0, al
    out 0x20, al
    
    push %1 ; push int num 
    call OsHwiHandle
    add esp, 4 ; skip int num

    call OsHwiTail
%endmacro


section .text
global OsHwiRet

OsHwiRet:
    add esp, 4 ; skip save_flag
    popad
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 4 ; skip error_code
    iret

OS_VECTOR 0x00, ZERO
OS_VECTOR 0x01, ZERO
OS_VECTOR 0x02, ZERO
OS_VECTOR 0x03, ZERO
OS_VECTOR 0x04, ZERO
OS_VECTOR 0x05, ZERO
OS_VECTOR 0x06, ZERO
OS_VECTOR 0x07, ZERO
OS_VECTOR 0x08, ERROR_CODE
OS_VECTOR 0x09, ZERO
OS_VECTOR 0x0a, ERROR_CODE
OS_VECTOR 0x0b, ERROR_CODE
OS_VECTOR 0x0c, ERROR_CODE
OS_VECTOR 0x0d, ERROR_CODE
OS_VECTOR 0x0e, ERROR_CODE
OS_VECTOR 0x0f, ZERO
OS_VECTOR 0x10, ZERO
OS_VECTOR 0x11, ERROR_CODE
OS_VECTOR 0x12, ZERO
OS_VECTOR 0x13, ZERO
OS_VECTOR 0x14, ZERO
OS_VECTOR 0x15, ZERO
OS_VECTOR 0x16, ZERO
OS_VECTOR 0x17, ZERO
OS_VECTOR 0x18, ZERO
OS_VECTOR 0x19, ZERO
OS_VECTOR 0x1a, ZERO
OS_VECTOR 0x1b, ZERO
OS_VECTOR 0x1c, ZERO
OS_VECTOR 0x1d, ZERO
OS_VECTOR 0x1e, ERROR_CODE
OS_VECTOR 0x1f, ZERO
OS_VECTOR 0x20, ZERO
OS_VECTOR 0x21, ZERO
OS_VECTOR 0x22, ZERO
OS_VECTOR 0x23, ZERO
OS_VECTOR 0x24, ZERO
OS_VECTOR 0x25, ZERO
OS_VECTOR 0x26, ZERO
OS_VECTOR 0x27, ZERO
OS_VECTOR 0x28, ZERO
OS_VECTOR 0x29, ZERO
OS_VECTOR 0x2a, ZERO
OS_VECTOR 0x2b, ZERO
OS_VECTOR 0x2c, ZERO
OS_VECTOR 0x2d, ZERO
OS_VECTOR 0x2e, ZERO
OS_VECTOR 0x2f, ZERO