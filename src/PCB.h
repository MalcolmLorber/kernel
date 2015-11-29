#ifndef _PCB_INC
#define _PCB_INC

#include <stdint.h>

typedef struct//all regs in proc
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t oesp; //what is this
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint16_t gs;
    uint16_t gspad;
    uint16_t fs;
    uint16_t fspad;
    uint16_t es;
    uint16_t espad;
    uint16_t ds;
    uint16_t dspad;

    uint32_t trap;

    uint32_t err;
    uint32_t eip;
    uint16_t cs;
    uint16_t cspad;

    uint32_t eflags;

    //ring changing, for user mode stuff. not used yet
    uint32_t esp;
    uint16_t ss;
    uint16_t sspad;
}__attribute__((packed)) trapframe;

typedef struct//regs needed for context
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
}__attribute__((packed)) context;

typedef struct
{
    uint32_t mem;
    volatile int pid;
    struct process* parent;
    trapframe* tf;
    context* ctxt;
    char name[32];
}__attribute__((packed)) process;

#endif
