/*
  elf.h

  Structures and symbols used for loading elf files
*/

#ifndef _ELF_INC
#define _ELF_INT

#include <stdint.h>

extern uint8_t elf_hello[]  asm("_binary_hello_elf_start");

#endif
