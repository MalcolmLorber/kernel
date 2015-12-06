#include "PCB.h"
#include "serial.h"
#include "term.h"
#define NUM_SYSCALLS 256

process* curproc;
process* otherproc;

void setproc(process* proc)
{
    curproc=proc; 
}

void schedule()
{
    context_switch(&curproc->ctxt, otherproc->ctxt);
    process* t = curproc;
    curproc = otherproc;
    otherproc = t;
}

int yield()
{
    //lock mutex
    curproc->state = RUNNABLE;
    schedule();
    //unlock mutex
    return 1;
}

int print1()
{
    terminal_writestring("1");
    return 1;
}

int print2()
{
    terminal_writestring("2");
    return 2;
}

int(*syscalls[NUM_SYSCALLS])()={0,0,0, yield, print1, print2};
/*NOPE, exit, fork, yield, test, test*/

void syscall()
{
    int num = curproc->tf->eax;
    
    if(num>0 && num<NUM_SYSCALLS && syscalls[num])
    {
        curproc->tf->eax = syscalls[num]();
    }
    else
    {
        serial_writestring("UNKNOWN SYSCALL CALLED\n");
        curproc->tf->eax = -1;
    }
}
