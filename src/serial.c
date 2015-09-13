/*
  serial.c has functions for handling and writing to the serial port. Often used for debugging
*/

#include "serial.h"

#define SERIAL_IO_PORT 0x3f8 // COM1

void serial_init() {
    outb(SERIAL_IO_PORT + 1, 0x00);    // Disable all interrupts
    outb(SERIAL_IO_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(SERIAL_IO_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_IO_PORT + 1, 0x00);    //                  (hi byte)
    outb(SERIAL_IO_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(SERIAL_IO_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_IO_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int is_transmit_empty() {
    return inb(SERIAL_IO_PORT + 5) & 0x20;
}

void serial_writechar(char a) {
    while (is_transmit_empty() == 0);

    outb(SERIAL_IO_PORT,a);
}

void serial_writestring(char* s) {
    while (*s) {
        serial_writechar(*s);
        s++;
    }
}
