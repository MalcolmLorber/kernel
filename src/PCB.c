#include "PCB.h"
#include "serial.h"
#include "term.h"
#define NUM_SYSCALLS 256
#define NUM_PROCS 16

process* procs[NUM_PROCS]; 
int curproc=-1;
int firstfree=0;

//process* curproc;
//process* otherproc;

void addproc(process* proc)
{
    procs[firstfree]=proc;
    firstfree++;
    //curproc=proc; 
}

void schedule()
{
    int old = curproc;
    curproc++;
    while(procs[curproc]==0 || procs[curproc]->state!=RUNNABLE)
    {
        curproc=(curproc<NUM_PROCS-1)?curproc+1:0;
    }
    context_switch(&procs[old]->ctxt, procs[curproc]->ctxt);
    //procs[old]->state=RUNNABLE;
    procs[curproc]->state=RUNNING;
    //process* t = curproc;
    //curproc = otherproc;
    //otherproc = t;
    
}

int yield()
{
    //lock mutex
    procs[curproc]->state = RUNNABLE;
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
    int num = procs[curproc]->tf->eax;
    
    if(num>0 && num<NUM_SYSCALLS && syscalls[num])
    {
        procs[curproc]->tf->eax = syscalls[num]();
    }
    else
    {
        serial_writestring("UNKNOWN SYSCALL CALLED\n");
        procs[curproc]->tf->eax = -1;
    }
}

void proc_setup()
{
    for(int i=0;i<NUM_PROCS;i++)
    {
        procs[i]=NULL;
    }
}
