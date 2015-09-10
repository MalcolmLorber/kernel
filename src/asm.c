/*
  asm.c contains x86 specific functions for executing low level code
*/

#include "asm.h"

// Wrapper for outb
// Stolen from http://wiki.osdev.org/Inline_Assembly/Examples
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Wrapper for inb
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
