/*
  elf.c

  Mainly, contains the elf loading code
*/

#include "elf.h"

void load_elf(elf_header* elf_start)
{
    // The elf will get its own memory space.
    page_directory_entry* pgdir = page_new_directory();

    void* highest_vpage = (void*)0;

    serial_writestring("Loading elf\n");

    // loop through and copy memory segments
    for (int i = 0; i < elf_start->ph_entry_count; i++)
    {
        serial_writestring("Loading memory segment\n");

        // get this section header
        elf_program_header* ph;
        ph = (elf_program_header*) ((uint32_t)elf_start + (uint32_t)(elf_start->ph_table_offset + (i * elf_start->ph_entry_size)));

        serial_hexword((uint32_t)elf_start);
        serial_writechar('\n');
        serial_hexword((uint32_t)ph);
        serial_writechar('\n');
        serial_hexword((uint32_t)elf_start + (uint32_t)(elf_start->ph_table_offset + (i * elf_start->ph_entry_size)));
        serial_writechar('\n');

        // map and copy
        for (void* pg = (void*)((uint32_t)ph->p_vaddr & 0xfffff000); ((int)pg - (int)ph->p_vaddr) < (int)ph->memory_segment_size; pg+=0x1000)
        {
            serial_writestring("Copying in memory segment\n");

            void* real_page = page_map(pgdir, pg);

            void* start = (pg > ph->p_vaddr) ? pg : ph->p_vaddr;
            void* end = (pg + 0x1000 < ph->p_vaddr + ph->file_segment_size) ? pg + 0x1000 : ph->p_vaddr + ph->file_segment_size;

            size_t size = end - start;
            size_t offset = (pg > ph->p_vaddr) ? 0 : ph->p_vaddr - pg;

            // do some memcpy stuff
            page_table_inspect(kernel_page_directory, real_page);
            memcpy(real_page + offset, start, size);

            // save the highest start
            if (pg > highest_vpage)
            {
                highest_vpage = pg;
            }
            break;
        }
    }

    // Map the kernel 1:1 in memory
    page_region(pgdir, 0x0, &_kernel_end);

    // Allocate two more pages for stack
    page_map(pgdir, highest_vpage + 0x1000);
    page_map(pgdir, highest_vpage + 0x2000);
    // initial esp = highest_vpage + 0x1000

    process* pcb = kmalloc(sizeof(process) + sizeof(context) + sizeof(trapframe));
    pcb->ctxt = (context*)((size_t)pcb + sizeof(process));
    pcb->tf = (trapframe*)((size_t)pcb + sizeof(process) + sizeof(context));

    // And of course, a kstack
    pcb->kstack = (char*)kmalloc(0x1000);

    pcb->mem = pgdir;
    pcb->pid = -1;
    pcb->parent = 0;
    pcb->kstack = 0;
    //pcb->name = "debugstuff";
    pcb->state = RUNNABLE;

    pcb->ctxt->edi = 0;
    pcb->ctxt->esi = 0;
    pcb->ctxt->ebx = 0;
    pcb->ctxt->ebp = (uint32_t)highest_vpage + 0x1000;
    pcb->ctxt->eip = (uint32_t)elf_start->program_entry_position;

    pcb->tf->ebp = (uint32_t)highest_vpage + 0x1000;
    pcb->tf->eip = (uint32_t)elf_start->program_entry_position;

    addproc(pcb);
    //asm("mov $3, %eax; int 0x80");
}
