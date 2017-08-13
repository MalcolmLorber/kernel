#include "comp.h"

//stack variables, should be dynamic later
int cstack[128];
int cstack_size;

//push int to stack
void cpush(int n)
{
    cstack[cstack_size]=n;
    cstack_size++;
}

//pop int off of stack, destroying it
void cpop()
{
    if(cstack_size==0)
	return;
    cstack_size--;
}

//peek at the top of the stack, returning it
int cpeek()
{
    return cstack[cstack_size-1];
}

//add the top two items on the stack, popping them and pushing the result on the stack
void cadd()
{
    if(cstack_size<2)
	return;
    int a=cpeek();
    cpop();
    int b=cpeek();
    cpop();
    cpush(a+b);
}

//multiply the top two items on the stack, popping them and pushing the result on the stack
void cmult()
{
    if(cstack_size<2)
	return;
    int a=cpeek();
    cpop();
    int b=cpeek();
    cpop();
    cpush(a*b);
}

//subtract the top two items on the stack, popping them and pushing the result on the stack
void csub()
{
    if(cstack_size<2)
	return;
    int a=cpeek();
    cpop();
    int b=cpeek();
    cpop();
    cpush(a-b);
}

//subtract the top two items on the stack in reverse order, popping them and pushing the result on the stack
void crsub()
{
    if(cstack_size<2)
	return;
    int a=cpeek();
    cpop();
    int b=cpeek();
    cpop();
    cpush(b-a);
}

//divide the top two items on the stack, popping them and pushing the result on the stack
void cdiv()
{
    if(cstack_size<2)
	return;
    int a=cpeek();
    cpop();
    int b=cpeek();
    cpop();
    cpush(a/b);
}

//divide the top two items on the stack in reverse order, popping them and pushing the result on the stack
void crdiv()
{
    if(cstack_size<2)
	return;
    int a=cpeek();
    cpop();
    int b=cpeek();
    cpop();
    cpush(b/a);
}
