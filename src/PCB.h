#ifndef _PCB_INC
#define _PCB_INC

#include <stdint.h>
#include "mem.h"

//This is the definitions for the Process Control Block, used for management of processes 

typedef struct//all registers needed to be stored in a process
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
    //uint32_t esp;
    //uint16_t ss;
    //uint16_t sspad;
}__attribute__((packed)) trapframe;

typedef struct//all registers needed for contexts
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
}__attribute__((packed)) context;

//possible process states
enum procstate {KILLED, RUNNABLE, RUNNING};

typedef struct//all the data that makes up a "process"
{
    page_directory_entry* mem;  //our process's allocated memory
    volatile int pid;           //our process's id
    struct process* parent;     //the process's parent (can be process that started this process)
    char* kstack;               //apparently we need this. idk why
    trapframe* tf;              //process trapframe for interrupts and errors
    context* ctxt;              //process context for switching and calling system interrupts
    char name[32];              //process name
    enum procstate state;       //process state
}__attribute__((packed)) process;

//context_switch is defined in switch.asm
extern void context_switch(context** old, context* new);
//some basic process management functions
void syscall();
void addproc(process* proc);
void settf(trapframe* tf);
void proc_setup();

#endif
