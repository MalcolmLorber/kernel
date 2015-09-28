#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>

/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#include <stdint.h>

#include "serial.h"
#include "term.h"
#include "mem.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() {
	/* Initialize terminal and serial interfaces */
	terminal_initialize();
    serial_init();

	/* Since there is no support for newlines in terminal_putchar
         * yet, '\n' will produce some VGA specific character instead.
         * This is normal.
         */
    serial_writestring("First Serial Test!\n");
    terminal_writestring("Hello, kernel World!\n");
    serial_writestring("Second Serial Test\n");

    // Enable paging of memory. For now there is only one virtual
    // memory space defined by page_dir
    uint32_t* page_dir;
    page_dir = initiate_directory();
    initiate_page_table(page_dir);
    loadPageDirectory(page_dir);
    enablePaging();

    serial_writestring("Finished Initilizing memory\n");
}
