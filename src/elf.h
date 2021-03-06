/*
  elf.h

  Structures and symbols used for loading elf files
*/

#ifndef _ELF_INC
#define _ELF_INT

#include <stdint.h>
#include <stddef.h>

#include "mem.h"
#include "string.h"
#include "serial.h"
#include "PCB.h"

extern uint8_t elf_hello[]  asm("_binary_hello_elf_start");

typedef struct
{
    uint32_t magic;     // \x7FELF
    uint8_t  word_size; // 32/64 bit
    uint8_t  endianness;
    uint8_t  elf_version;
    uint8_t  os_abi;
    uint64_t unused;
    uint16_t relocatable;
    uint16_t instruction_set;
    uint32_t elf_version_b;
    void*    program_entry_position;
    uint32_t ph_table_offset;
    uint32_t sh_table_offset;
    uint32_t flags;  // undefined in x86
    uint16_t header_size;
    uint16_t ph_entry_size;
    uint16_t ph_entry_count;
    uint16_t sh_entry_size;
    uint16_t sh_entry_count;
    uint16_t sh_name_index;
} __attribute__((packed)) elf_header;

typedef struct
{
    uint32_t segment_type;
    uint32_t p_offset; // Offset in the elf file for this segment's data
    void*    p_vaddr;  // Where this segment should be located in virtual memory
    uint32_t unused;
    uint32_t file_segment_size;   // Size of the segment in this file and in memory
    uint32_t memory_segment_size;
    uint32_t flags;
    uint32_t alignment;
} elf_program_header;

// Will probably return a pointer to the PCB block eventually. Dunno though.
void load_elf(elf_header* elf_start);

#endif
