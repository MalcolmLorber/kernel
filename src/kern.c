/*
  kern.c is where the c code starts, as invoked by our heading
  assembly. It should only contain high level code
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "string.h"
#include "serial.h"
#include "term.h"
#include "mem.h"
#include "gdt.h"
#include "idt.h"
#include "test.h"
#include "pci.h"
#include "multiboot.h"
#include "pic.h"
#include "pit.h"
#include "io.h"

// The parameters passed here ultimately come through the bootloader
void kernel_main(multiboot_info* mbt, uint32_t magic)
{
    /* Initialize terminal and serial interfaces */
    terminal_initialize();
    serial_init();
    gdt_init();
    idt_init(0x8);
    serial_writestring("IDT initialized\n");
    disable_int();
    pic_init(0x20,0x28);
    pit_init();
    io_init();
    pit_start_counter(100, PIT_OCW_CONT_0, PIT_OCW_MODE_SQWVGEN);
    enable_int();
    
	/* Since there is no support for newlines in terminal_putchar
         * yet, '\n' will produce some VGA specific character instead.
         * This is normal.
         */
    
    serial_writestring("Hello, kernel World!\n");
    terminal_writestring("Hello, kernel World!\n");

    // Abort if magic is not correct.
    if (magic != 0x2badb002)
    {
        serial_writestring("Magic number not correct\n");
        // wait how do we terminate
    }

    // Enable paging of memory. For now there is only one virtual
    // memory space defined by page_dir
    page_directory_entry* kern_page_dir;
    kern_page_dir = mem_init_kern_tables(mbt->mmap_addr, mbt->mmap_addr + mbt->mmap_length);
    loadPageDirectory(kern_page_dir);
    enablePaging();

    serial_writestring("Finished Initilizing memory\n");
    int o=0;
    int j=5/o;
    serial_hexword(j);    
    test_mem();

    // PCI
    checkAllBuses();

    test_idt();
    while(true)
    {
	asm("hlt");
    }
}
