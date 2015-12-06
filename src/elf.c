/*
  elf.c

  Mainly, contains the elf loading code
*/

#include "elf.h"

void load_elf(elf_header* elf_start)
{
    // The elf will get its own memory space.
    page_directory_entry* pgdir = page_new_directory();

    // loop through and copy memory segments
    for (int i = 0; i < elf_start->ph_entry_count; i++)
    {
        // get this section header
        elf_program_header* ph;
        ph = (elf_program_header*) elf_start + (elf_start->ph_table_offset + (i * elf_start->ph_entry_size));

        // map and copy
        for (void* pg = (void*)((uint32_t)ph->p_vaddr & 0xfffff000); (uint32_t)(pg - ph->p_vaddr) < ph->memory_segment_size; pg+=0x1000)
        {
            void* real_page = page_map(pgdir, pg);

            void* start = (pg > ph->p_vaddr) ? pg : ph->p_vaddr;
            void* end = (pg + 0x1000 < ph->p_vaddr + ph->file_segment_size) ? pg + 0x1000 : ph->p_vaddr + ph->file_segment_size;

            size_t size = end - start;
            size_t offset = (pg > ph->p_vaddr) ? 0 : ph->p_vaddr - pg;

            // do some memcpy stuff
            memcpy(real_page + offset, start, size);
        }
    }
}
