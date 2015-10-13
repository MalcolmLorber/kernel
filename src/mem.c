/*
  mem.c is the main file for dealing with virtual memory and paging.
*/

#include "mem.h"
#include "serial.h"

// page_directory is currently defined once and should not be touched
// later. It must be page aligned for the MMU to read properly
page_directory_entry page_directory[1024] __attribute__((aligned(4096)));

// page_table_one is the first page table, and at the beginning the
// only page table in the page directory
page_table_entry page_table_one[1024] __attribute__((aligned(4096)));

page_directory_entry* initiate_directory() 
{
    // set each entry to not present
    int i;
    for(i = 0; i < 1024; i++)
    {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //     (this is the 2 bit)
        //   Not Present: The page table is not present
        page_directory[i] = 0;
        page_directory[i] |= PAGE_WRITABLE;
    }

    // Set last directory entry to point back
    page_directory[1023] = (uint32_t) page_directory;
    page_directory[1023] |= PAGE_WRITABLE | PAGE_PRESENT;

    return page_directory;
}

void initiate_page_table(page_directory_entry* page_dir) 
{
    // holds the physical address where we want to start mapping these pages to.
    // in this case, we want to map these pages to the very beginning of memory.
    unsigned int i;

    //we will fill all 1024 entries in the table, mapping 4 megabytes
    for(i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes:
        //    -> supervisor level, read/write, present.
        page_table_one[i] = 0;
        page_table_one[i] |= i << 12;
        page_table_one[i] |= PAGE_WRITABLE | PAGE_PRESENT;
    }

    // attributes: supervisor level, read/write, present
    page_dir[0] = (uint32_t) page_table_one;
    page_dir[0] |= PAGE_WRITABLE | PAGE_PRESENT;

}
