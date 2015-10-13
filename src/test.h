/*
  test.h defines functions used for testing
 */

#ifndef INC_TEST
#define INC_TEST

#include "serial.h"

// test_idt calls a couple of interrupts. Should be done after kernel
// has been set up
void test_idt();

// test_mem writes some stuff to moem
void test_mem();

#endif
