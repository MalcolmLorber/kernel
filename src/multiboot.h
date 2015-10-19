/*
  multiboot.h defines structures used in the multiboot standard

  much of this file was closely inspired by the gnu version
*/

#ifndef _MULTIBOOT_INC
#define _MULTIBOOT_INC

#include <stdint.h>

typedef struct
{
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} multiboot_memory_map;

typedef struct
{
    uint32_t tabsize;
    uint32_t strsize;
    uint32_t addr;
    uint32_t reserved;
} aout_symbol_table;

typedef struct
{
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    union
    {
        aout_symbol_table aout_sym;
    } u;
    uint32_t mmap_length;
    multiboot_memory_map* mmap_addr;

} multiboot_info;



#endif
