/*
  elf.c

  Mainly, contains the elf loading code
*/

#include "mem.h"

void load_elf(elf_header* elf_start)
{
    // The elf will get its own memory space.
    page_directory_entry* pgdir = page_new_directory();

    // loop through and copy memory segments
    for (int i = 0; i < elf_start->ph_entry_count; i++)
    {
        // get this section header
        elf_program_header* ph;
        ph = elf_start + elf_start->ph_table_offset + (i * elf_start->ph_entry_size);

        // map and copy
        for (pg = ph->p_vaddr & 0xfffff000; pg - ph->p_vaddr < ph->memory_segment_size; pg+=0x1000)
        {
            real_page = page_allocate(pg);

            start = (pg > ph->p_vaddr) ? pg : ph->p_vaddr;
            end = (pg + 0x1000 < ph->p_vaddr + ph->file_segment_size) ? pg + 0x1000 : ph->p_vaddr + ph->file_segment_size;

            size = end - start;
            offset = (pg > ph->p_vaddr) ? 0 : ph->p_vaddr - pg;

            // do some memcpy stuff
            memcpy(real_page + offset, start, size);

            elf_start + p->ph_offset
        }
    }

}
