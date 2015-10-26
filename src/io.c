#include "io.h"
#include "pic.h"
#include "serial.h"
#include "idt.h"
#include "string.h"

void kbd_irq()
{
    serial_hexword(inb(0x60));
    serial_writestring("\n");
    pic_command(0, PIC_OCW2_EOI);
}

void io_init()
{
    install_c_ir(33, kbd_irq);
}
