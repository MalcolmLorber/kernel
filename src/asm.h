/*
  asm.h contains x86 specific functions for executing low level code
*/

#pragma once
#include <stdint.h>

// Wrapper for outb
// Stolen from http://wiki.osdev.org/Inline_Assembly/Examples
inline void outb(uint16_t port, uint8_t val) 
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Wrapper for inb
inline uint8_t inb(uint16_t port) 
{
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

inline void disable_int()
{
    asm volatile ("cli");
}

inline void enable_int()
{
    asm volatile ("sti");
}
