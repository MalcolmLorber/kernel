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

// Defined in linker.ld
extern char _kernel_end[];

// Allocation of pages
void* page_allocate();
void page_free(void* page_start);
void* page_map(page_directory_entry pgdir[], void* page_start);

// Initilization
page_directory_entry* mem_init_kern_tables(multiboot_memory_map* mmap, multiboot_memory_map* mmap_end);

// Directory and table handling
page_directory_entry* page_new_directory();
page_table_entry* page_new_table();
page_directory_entry* initiate_directory(); // DEPRECATED

// memory mark stores a counter of bytes for the most recently used
// memory. Everything before it is used and all valid memory after it
// is free
void* memory_mark;

void* malloc(uint32_t bytes);

// Required for elf loading - virtual memory and stuff

// TODO:
/*
  Two stage memory thing.

  Ensure kernel is paged.
  Ensure that a space equal to <remaining_memory> / (4096 * 8) is paged. This is for the following struct
  Create binary array specifying which pages are used (abstract ownership a bit maybe to pid, or just use bit array)
    Place in allocated space

  Write allocor for pages. This should be the only way to get ownership of pages.
    - takes a page directory and a new address as an argument.
    - Sholud be called by the other malloc function, or by construction of page tables
    - flips a bit in a binary array

    - Also write corresponding free, flip bit and remove entry in page dir/table

  Write more versatile malloc/free
    - Use struct{void* next, size_t size} before every hunk. Walk list to find total size, use this to determine if more pages are needed. Or something


    Problems - how to determine when new pages are needed
             - how to load in elf stuff at arbitrary locations
*/

#endif
