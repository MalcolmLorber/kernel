#include <stdbool.h> /* C doesn't have booleans by default. */
#include <stddef.h>
#include <stdint.h>

#include "string.h"
#include "serial.h"
#include "term.h"
#include "mem.h"
#include "gdt.h"
#include "idt.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() {
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
    //serial_writestring("Second Serial Test\n");

    // Enable paging of memory. For now there is only one virtual
    // memory space defined by page_dir
    page_directory_entry* page_dir;
    page_dir = initiate_directory();
    initiate_page_table(page_dir);
    loadPageDirectory(page_dir);
    enablePaging();

    serial_writestring("Finished Initilizing memory\n");
    asm ("int $0x80");
    asm ("int $0x90");
    asm ("hlt");
}
