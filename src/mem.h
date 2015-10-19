/*
  mem.h
*/

#ifndef _MEM_INC
#define _MEM_INT

#include <stdbool.h>
#include <stdint.h>

#include "multiboot.h"

// Flags for pages directory entries and table entries
#define PAGE_PRESENT        0x1
#define PAGE_WRITABLE       0x2
#define PAGE_USER           0x4
#define PAGE_WRITE_THROUGH  0x8
#define PAGE_CACHE_DISABLED 0x10
#define PAGE_ACCESSED       0x20
#define PAGE_DIRTY          0x80
#define PAGE_SIZE_FLAG      0x80

typedef uint32_t page_directory_entry;
typedef uint32_t page_table_entry;

// These two are defined in the assembly file paging.asm
extern void loadPageDirectory(page_directory_entry*);
extern void enablePaging();

// Defined in the file mem.c
page_directory_entry* initiate_directory();
page_directory_entry* mem_init_kern_tables(multiboot_memory_map* mmap, multiboot_memory_map* mmap_end);

// memory mark stores a counter of bytes for the most recently used
// memory. Everything before it is used and all valid memory after it
// is free
void* memory_mark;

void* malloc(uint32_t bytes);

#endif
