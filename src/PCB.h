#ifndef _PCB_INC
#define _PCB_INC

#include <stdint.h>
#include "mem.h"

void* kstack;

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

    //uint32_t mmm;
    //uint32_t why;

    uint32_t err;

    uint32_t eip;
    uint16_t cs;
    uint16_t cspad;

    uint32_t eflags;

    //ring changing, for user mode stuff. not used yet
    //uint32_t esp;
    //uint16_t ss;
    //uint16_t sspad;
}__attribute__((packed)) trapframe;

typedef struct//regs needed for context
{
    uint32_t cr3;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
}__attribute__((packed)) context;

enum procstate {KILLED, RUNNABLE, RUNNING};

typedef struct
{
    page_directory_entry* mem;
    volatile int pid;
    struct process* parent;
    char* kstack; //apparently we need this. idk why
    trapframe* tf;
    context* ctxt;
    char name[32];
    enum procstate state;
}__attribute__((packed)) process;

extern void context_switch(context** old, context* new);
void syscall();
void addproc(process* proc);
void settf(trapframe* tf);
void proc_setup();
int yield();
#endif
