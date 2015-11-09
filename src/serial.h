/*
  serial.h contains headers for the serial interaction functions
*/

#ifndef _SERIAL_INC
#define _SERIAL_INC

#include "asm.h"
#include <stdint.h>

// Initilizes the first serial port
void serial_init();

// Tests that the serial transmit channel is free
int is_transmit_empty();

// Write a single character to serial
void serial_writechar(char a);

// Write a null terminated string to serial
void serial_writestring(char* s);

// Function to write a 4 byte word toserial in hex
void serial_hexbyte(uint8_t x);
void serial_hexhword(uint16_t x);
void serial_hexword(uint32_t x);
void serial_hexqword(uint64_t x);

// Writes an arbitrary amount memory to serial
void serial_hexstring(void* s, uint32_t n);

#endif
