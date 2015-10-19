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
#include "multiboot.h"

// The parameters passed here ultimately come through the bootloader
void kernel_main(multiboot_info* mbt, uint32_t magic)
{
    /* Initialize terminal and serial interfaces */
    terminal_initialize();
    serial_init();
    gdt_init();
    idt_init(0x8);

	/* Since there is no support for newlines in terminal_putchar
         * yet, '\n' will produce some VGA specific character instead.
         * This is normal.
         */
    serial_writestring("IDT initialized\n");
    terminal_writestring("Hello, kernel World!\n");

    // Abort if magic is not correct.
    if (magic != 0x2badb002)
    {
        serial_writestring("Magic number not correct\n");
        // wait how do we terminate
    }

    // Enable paging of memory. For now there is only one virtual
    // memory space defined by page_dir
    page_directory_entry* page_dir;
    page_dir = initiate_directory();
    initiate_page_table(page_dir);
    loadPageDirectory(page_dir);
    enablePaging();

    serial_writestring("Finished Initilizing memory\n");
    int o=0;
    int j=5/o;
    serial_hexword(j);
    test_mem();
    test_idt();
}
