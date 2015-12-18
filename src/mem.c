/*
  mem.c is the main file for dealing with virtual memory and paging.
*/

#include "mem.h"
#include "serial.h"

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

// page_allocate returns the address of a new page in real memory
void* page_allocate()
{
    // The starting address of the map should always be the same: &_kernel_end

    // Find the correct bit to flip and page number
    uint8_t* index = (uint8_t*) &_kernel_end;
    while (*index == 0xff) index++;
    uint8_t i = 7;
    while ((*index & (1<<i)) != 0) i--;

    // bitfilp
    *index = *index | (1<<i);

    uint32_t page_number = ((uint32_t)(index - (uint8_t*)_kernel_end) * 8) + (8-i);
    return (void*)&_kernel_end + 0x1000 * page_number;
}

// page_free just flips the bit to say that the page has been freed
void page_free(void* page_start)
{
    if (((uint32_t)page_start & 0xfff) != 0)
    {
        serial_writestring("tried to free a non-page as a page");
    }

    uint32_t page_number = (uint32_t)(page_start - (void*)&_kernel_end) / 0x1000;
    uint8_t* index = (uint8_t*) &_kernel_end + page_number / 8;
    *index = ~(~*index | (1<<(8-(page_number%8))));
}

// Just pages the entire addressable space. Takes about 4MB of space.
// Should be ~fine~ this won't break things I totally know what I'm doing
void page_everything(page_directory_entry* pg_dir)
{
    for (int i = 0; i < 1024; i++)
    {
        page_table_entry* pg_tab = (page_table_entry*) page_allocate();
        pg_dir[i] = (page_directory_entry)((uint32_t)pg_tab | PAGE_WRITABLE | PAGE_PRESENT);
        for (int j = 0; j < 1024; j++)
        {
            void* page = (void*)((i << 22) | (j << 12));
            pg_tab[j] = (page_directory_entry)((uint32_t)page | PAGE_WRITABLE | PAGE_PRESENT);
        }
    }
}

// page_region adds page tables to map to real memory for a region
void page_region(page_directory_entry pgdir[], void* begin, void* end)
{
    for (void* page = begin; page < end; page += 0x1000)
    {
        if ((pgdir[(uint32_t)page>>22] & 0xfffff000) == 0)
        {
            pgdir[(uint32_t)page>>22] |= (page_directory_entry)page_new_table();
        }
        page_table_entry* page_table = (page_table_entry*) ((uint32_t) pgdir[(uint32_t)page>>22] & 0xfffff000);
        if ((page_table[((uint32_t)page >> 12) % 1024] & 0xfffff000) == 0)
        {
            page_table[((uint32_t)page>>12)%1024] |= (page_table_entry)page;
        }
    }
}

// This goes through all available memory, and maps 1 to 1 virtual memory to it.
page_directory_entry* mem_init_kern_tables(multiboot_memory_map* mmap, multiboot_memory_map* mmap_end)
{
    // Allocate enough space for the biggest possible allocation bitmap. Assigning 1 bit to each
    for (int i = 0; i < 32; i++)
    {
        page_allocate();
    }

    // Initilize a directory mapped to everything
    page_directory_entry* kern_page_dir = kernel_page_directory;
    page_everything(kern_page_dir);

    // _kern_end is defined in the linker script
    /* page_table_entry* next_table = (page_table_entry*) &_kernel_end; */

    //int current_dir_entry = 0;

    uint32_t totalmem = 0;

    // For each entry in the memory map
    while (mmap < mmap_end)
    {
        //void* next_table = page_allocate();
        // Special case for the first memory segment - it gets its own directory entry
        if (mmap->base_addr == 0)
        {
            // Lower memory should be first in the page directory and
            // mapped one to one
            /* populate_page_table(next_table, 0, 1024, 0); */
            /* kern_page_dir[0] = (uint32_t) next_table; */
            /* kern_page_dir[0] |= PAGE_WRITABLE | PAGE_PRESENT; */

            /* current_dir_entry = 1; */
            /* next_table += 0x1000; */
        }
        else if (mmap->type == 1)
        {
            totalmem += mmap->length;
            /* uint32_t i; */
            /* for (i = 0; i+0x1000000 < mmap->length; i += 0x1000000) */
            /* { */
            /*     populate_page_table(next_table, (void*)(intptr_t) mmap->base_addr + i, 1024 ,0); */
            /*     kern_page_dir[current_dir_entry] = (uint32_t) next_table; */
            /*     kern_page_dir[current_dir_entry] |= PAGE_WRITABLE | PAGE_PRESENT; */

            /*     current_dir_entry += 1; */
            /*     /\* next_table += 0x1000; *\/ */
            /* } */
            // There will be an edge case where there is less than a
            // full page table to be mapped. This handlis this case.
            /* if (i != mmap->length) */
            /* { */
            /*     populate_page_table(next_table, (void*)(intptr_t) mmap->base_addr + i, (mmap->length - i) / 0x1000 ,0); */
            /*     kern_page_dir[current_dir_entry] = (uint32_t) next_table; */
            /*     kern_page_dir[current_dir_entry] |= PAGE_WRITABLE | PAGE_PRESENT; */

            /*     current_dir_entry += 1; */
            /*     /\* next_table += 0x1000; *\/ */
            /* } */

        }
        mmap = (multiboot_memory_map*)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
    }

    // let the memory allocator work freely with all remaining memory
    /* memory_mark = page_allocate(); */

    return kern_page_dir;
}

// page_map takes in a virtual address and a directory, and ensures
// that that address points to valid memory
void* page_map(page_directory_entry pgdir[], void* page_start)
{
    void* page = 0;
    if ((pgdir[(uint32_t)page_start>>22] & 0xfffff000) == 0)
    {
        pgdir[(uint32_t)page_start>>22] |= (page_directory_entry)page_new_table();
    }
    page_table_entry* page_table = (page_table_entry*) ((uint32_t) pgdir[(uint32_t)page_start>>22] & 0xfffff000);
    if ((page_table[((uint32_t)page_start >> 12) % 1024] & 0xfffff000) == 0)
    {
        page = page_allocate();
        page_table[((uint32_t)page_start>>12)%1024] |= (page_table_entry)page;
    }

    return page;
}


page_directory_entry* page_new_directory()
{
    page_directory_entry* pgdir = (page_directory_entry*) page_allocate();
    int i;
    for(i = 0; i < 1024; i++)
    {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //     (this is the 2 bit)
        //   Not Present: The page table is not present
        pgdir[i] = 0;
        pgdir[i] |= PAGE_WRITABLE;
    }
    return pgdir;
}

// Returns a blank table
page_table_entry* page_new_table()
{
    page_table_entry* pgtab = (page_table_entry*) page_allocate();
    int i;
    for(i = 0; i < 1024; i++)
    {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //     (this is the 2 bit)
        //   Not Present: The page table is not present
        pgtab[i] = 0;
        pgtab[i] |= PAGE_WRITABLE;
    }
    return pgtab;
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
        kernel_page_directory[i] = 0;
        kernel_page_directory[i] |= PAGE_WRITABLE;
    }

    // Set last directory entry to point back
    kernel_page_directory[1023] = (uint32_t) kernel_page_directory;
    kernel_page_directory[1023] |= PAGE_WRITABLE | PAGE_PRESENT;

    return kernel_page_directory;
}

void* malloc(uint32_t bytes)
{
    void* old_mark = memory_mark;
    memory_mark = (void*)((uint32_t)memory_mark + bytes);
    return old_mark;
}


void* kmalloc(uint32_t size __attribute__((unused)))
{
    return page_allocate();
}


// Debugging
void* page_table_inspect(page_directory_entry* pg_dir, void* addr)
{
    serial_writestring("Inspecting page tables for address 0x");
    serial_hexword((uint32_t)addr);
    serial_writestring("\nDirectory Entry: 0x");
    page_directory_entry page_dir_entry = pg_dir[((uint32_t)addr) >> 22];
    serial_hexword(page_dir_entry);
    serial_writestring("\nPage Table Entry: 0x");
    page_table_entry* page_table = (page_table_entry*)((uint32_t) page_dir_entry & 0xfffff000);
    page_table_entry pg_tab_entry = page_table[((uint32_t)addr >> 12) % 1024];
    serial_hexword(pg_tab_entry);
    serial_writechar('\n');

    return (void*)((uint32_t)pg_tab_entry & 0xfffff000);
}
