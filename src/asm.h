/*
  asm.h exports archatecture specific functions for hardware interaction
*/

#include <stdint.h>

// Functions for reading and writing to/from x86 IO ports
static inline void outb(uint16_t port, uint8_t val);
static inline uint8_t inb(uint16_t port);
