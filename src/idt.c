#include "idt.h"
#include "string.h"
#include "serial.h"

uint16_t idt_limit;
uint32_t idt_base;

idt_desc _idt [MAX_IDT_INT];

void idt_install() {
    struct{
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) idtr;
    idtr.limit = idt_limit;
    idtr.base = idt_base;
    char f[20];
    itoa(idtr.limit, f);
    itoa(idtr.base, f);
    asm volatile ("lidt (%0)": :"r"(&idtr));
}

static void default_handler () {
    //DEBUG OUTPUT MAYBE?
    serial_writestring("int caught\n");
    return;
}

idt_desc* get_ir (uint32_t i) {
    if (i>MAX_IDT_INT)
	return 0;
    return &_idt[i];
}

int install_ir (uint32_t i, uint16_t flags, uint16_t code_sel, IRQ_HANDLER irq) {
    if (i>MAX_IDT_INT)
	return 0;
    if (!irq)
	return 0;
    uint64_t uiBase = (uintptr_t)(&(*irq));
    _idt[i].low_base  = (uint16_t)(uiBase & 0xffff);
    _idt[i].high_base = (uint16_t)((uiBase >> 16) & 0xffff);
    _idt[i].reserved  = 0;
    _idt[i].flags	    = (uint8_t)(flags);
    _idt[i].code_sel  = code_sel;
    return 0;
}

int idt_init (uint16_t code_sel) {
    idt_limit = sizeof (idt_desc) * MAX_IDT_INT -1;
    idt_base  = (uint32_t)&_idt[0];
    memset ((void*)&_idt[0], 0, sizeof (idt_desc) * MAX_IDT_INT-1);
    for (int i=0; i<MAX_IDT_INT; i++)
	install_ir (i, IDT_PR|IDT_32,code_sel,(IRQ_HANDLER)default_handler);
    idt_install();
    char f[20];
    itoa((uintptr_t)&_idt,f);
    //serial_writestring(f);
    return 0;
}
