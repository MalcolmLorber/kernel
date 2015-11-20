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

// populate_page_table makes a page table at `table` mapping `num`
// pages to bytes after `start + offset*4KB`
// both table and start must be properly aligned
void populate_page_table(page_table_entry* table, void* start, int num, int offset)
{
    // Check that there are the correct number of entries
    if ((offset + num) > 1024)
    {
        serial_writestring("tred to populate past the end of the page table\n");
    }

    // Check that the table is properly aligned
    if (((uint32_t)table & 0xfff) != 0)
    {
        serial_writestring("tried to populate a table that was not page aligned");
    }

    // Check that the table is properly aligned
    if (((uint32_t)start & 0xfff) != 0)
    {
        serial_writestring("tried to start mapping on an unaligned address");
    }

    int i;
    int stop = num + offset;
    for (i = offset; i < stop; i++)
    {
        table[i] = 0;
        table[i] |= (uint32_t)start + (i<<12);
        table[i] |= PAGE_WRITABLE | PAGE_PRESENT;
    }
}

// TODO: put this as a label at the end of the code, so we can take
// its address
#define KERN_END 0x204000

page_directory_entry* mem_init_kern_tables(multiboot_memory_map* mmap, multiboot_memory_map* mmap_end)
{

    // Make a page directory structure easily
    page_directory_entry* kern_page_dir = initiate_directory();

    // This line will change with the above TODO
    page_table_entry* next_table = (page_table_entry*) KERN_END;

    int current_dir_entry = 0;

    // For each entry in the memory map
    while (mmap < mmap_end)
    {
        // Special case for the first memory segment - it gets its own directory entry
        if (mmap->base_addr == 0)
        {
            // Lower memory should be first in the page directory and
            // mapped one to one
            populate_page_table(next_table, 0, 1024, 0);
            page_directory[0] = (uint32_t) next_table;
            page_directory[0] |= PAGE_WRITABLE | PAGE_PRESENT;

            current_dir_entry = 1;
            next_table += 0x1000;
        }
        else if (mmap->type == 1)
        {
            uint32_t i;
            for (i = 0; i+0x1000000 < mmap->length; i += 0x1000000)
            {
                populate_page_table(next_table, (void*) mmap->base_addr + i, 1024 ,0);
                page_directory[current_dir_entry] = (uint32_t) next_table;
                page_directory[current_dir_entry] |= PAGE_WRITABLE | PAGE_PRESENT;

                current_dir_entry += 1;
                next_table += 0x1000;
            }
            // There will be an edge case where there is less than a
            // full page table to be mapped. This handlis this case.
            if (i != mmap->length)
            {
                populate_page_table(next_table, (void*) mmap->base_addr + i, (mmap->length - i) / 0x1000 ,0);
                page_directory[current_dir_entry] = (uint32_t) next_table;
                page_directory[current_dir_entry] |= PAGE_WRITABLE | PAGE_PRESENT;

                current_dir_entry += 1;
                next_table += 0x1000;
            }

        }
        mmap = (multiboot_memory_map*)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
    }

    // let the memory allocator work freely with all remaining memory
    memory_mark = next_table;

    return kern_page_dir;
}

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

// Expect addr to be page aligned
void mem_map_directory_entry(void* addr, page_directory_entry* page_dir) {
    page_table_entry* new_page_table = (page_table_entry*) (((uint32_t)malloc(2048)+1024)&0xfffffc00);
    populate_page_table(new_page_table, (uint32_t)addr & 0xffc00000, 1024, 0);
    page_dir[(uint32_t)addr >> 22] = (uint32_t) new_page_table;
    page_dir[(uint32_t)addr >> 22] |= PAGE_WRITABLE | PAGE_PRESENT;
}

void* malloc(uint32_t bytes)
{
    void* old_mark = memory_mark;
    memory_mark = (void*)((uint32_t)memory_mark + bytes);
    return old_mark;
}
