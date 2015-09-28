/*
  mem.h
*/

#ifndef _MEM_INC
#define _MEM_INT

#include <stdbool.h>
#include <stdint.h>

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
void initiate_page_table(page_directory_entry* page_dir);

#endif
