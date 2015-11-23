/*
  serial.c has functions for handling and writing to the serial port. Often used for debugging
*/

#include "serial.h"

#define SERIAL_IO_PORT 0x3f8 // COM1

void serial_init()
{
    outb(SERIAL_IO_PORT + 1, 0x00);    // Disable all interrupts
    outb(SERIAL_IO_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(SERIAL_IO_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_IO_PORT + 1, 0x00);    //                  (hi byte)
    outb(SERIAL_IO_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(SERIAL_IO_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_IO_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int is_transmit_empty()
{
    return inb(SERIAL_IO_PORT + 5) & 0x20;
}

void serial_writechar(char a)
{
    while (is_transmit_empty() == 0);

    outb(SERIAL_IO_PORT,a);
}

void serial_writestring(char* s)
{
    while (*s)
    {
        serial_writechar(*s);
        s++;
    }
}

const char* hexchars = "0123456789abcdef";
void serial_hexbyte(uint8_t x)
{
    for (int i = 1; i >= 0; i--)
    {
        serial_writechar(hexchars[x>>i*4&0xf]);
    }
}

void serial_hexhword(uint16_t x)
{
    for (int i = 3; i >= 0; i--)
    {
        serial_writechar(hexchars[x>>i*4&0xf]);
    }
}

void serial_hexword(uint32_t x)
{
    for (int i = 7; i >= 0; i--)
    {
        serial_writechar(hexchars[x>>i*4&0xf]);
    }
}

void serial_hexqword(uint64_t x)
{
    for (int i = 15; i >= 0; i--)
    {
        serial_writechar(hexchars[x>>i*4&0xf]);
    }
}

void serial_hexstring(void* s, uint32_t n) {
    char* d = (char*)s;
    for (uint32_t i = 0; i < n; i++)
    {
        serial_writechar(hexchars[(d[i]>>4)&0xf]);
        serial_writechar(hexchars[d[i]&0xf]);
    }
}
