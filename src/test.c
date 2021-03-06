/*
  test.c implements functions used for testing
*/

#include "test.h"

void test_idt()
{
    asm volatile("mov $4, %eax; int $0x80");
    asm volatile("int $0x90");
}

void test_mem()
{
    // try writing a bunch of bytes starting at 0
    int i = 0x0;
    for(i=0; i < 0x4000; i++)
    {
        *(int*)(i<<2) = i;
    }

    // read it back in and make sure its all stored

    for (i=0; i < 0x4000; i++)
    {
        if (*(int*)(i<<2) != i)
        {
            serial_writestring("Memory error\n");
        }
    }

    serial_writestring("Wrote a bunch of memory\n");
}
