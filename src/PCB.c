#include "PCB.h"
#include "serial.h"
#include "term.h"
#define NUM_SYSCALLS 256
#define NUM_PROCS 16

process* procs[NUM_PROCS]; 
int curproc=0;
int firstfree=0;

//process* curproc;
//process* otherproc;

//add a process to the currently running processes
void addproc(process* proc)
{
    procs[firstfree]=proc;
    firstfree++;
    //curproc=proc; 
}

//set a processes trapframe
void settf(trapframe* tf)
{
    procs[curproc]->tf = tf;
}

//schedule time for a process to run
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

//allow a process to yield its time if it is waiting on i/o, etc
int yield()
{
    //lock mutex
    procs[curproc]->state = RUNNABLE;
    schedule();
    //unlock mutex
    return 1;
}

//a test syscall
int print1()
{
    terminal_writestring("1\n");
    return 1;
}

//another test syscall
int print2()
{
    terminal_writestring("2\n");
    return 2;
}

//list of all syscalls we support
int(*syscalls[NUM_SYSCALLS])()={0,0,0, yield, print1, print2};
/*NOPE, exit, fork, yield, test, test*/

//delegation function for determining what syscall to call form a process
void syscall()
{
    int num = procs[curproc]->tf->eax;
    
    serial_hexword(num);
    serial_writestring(" SYSCALLED\n");
    
    if(num>0 && num<NUM_SYSCALLS && syscalls[num])
    {
        procs[curproc]->tf->eax = syscalls[num]();
    }
    else
    {
        serial_writestring("UNKNOWN SYSCALL CALLED ");
        serial_hexword(num);
        serial_writestring("\n");
        procs[curproc]->tf->eax = -1;
    }
}

//initilization of our process table, for now just a memset
void proc_setup()
{
    for(int i=0;i<NUM_PROCS;i++)
    {
        procs[i]=NULL;
    }
}
