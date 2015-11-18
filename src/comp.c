#include "comp.h"

int cstack[128];
int cstack_size;

void cpush(int n)
{
    cstack[cstack_size]=n;
    cstack_size++;
}

void cpop()
{
    if(cstack_size==0)
	return;
    cstack_size--;
}

int cpeek()
{
    return cstack[cstack_size-1];
}

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
