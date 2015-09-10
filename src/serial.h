/*
  serial.h contains headers for the serial interaction functions
*/

#include "asm.h"

// Initilizes the first serial port
void serial_init();

// Tests that the serial transmit channel is free
int is_transmit_empty();

// Write a single character to serial
void serial_writechar(char a);

// Write a null terminated string to serial
void serial_writestring(char* s);
