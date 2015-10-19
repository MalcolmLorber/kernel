#include "idt.h"
#include "string.h"
#include "serial.h"

#pragma GCC push_options
#pragma GCC optimize ("0")

#define serial_val(i) itoa(i,f);serial_writestring(f);serial_writestring(" ");
uint16_t idt_limit;
uint32_t idt_base;

idt_desc _idt [MAX_IDT_INT];

uint16_t _code_sel;

void idt_install() 
{
    struct
    {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) idtr;
    idtr.limit = idt_limit;
    idtr.base = idt_base;
    asm volatile ("lidt (%0)": :"r"(&idtr));
}

void default_handler(struct reg_state reg, uint32_t interrupt, uint32_t error, struct stack_state stack) 
{
    serial_writestring("int caught ");
    char f[20];
    serial_val(interrupt);
    serial_val(error);
    serial_val(stack.eip);
    serial_val(stack.cd);
    serial_val(stack.eflags);
    serial_writestring("\n");
    return;
}

idt_desc* get_ir(uint32_t i) 
{
    if (i > MAX_IDT_INT)
    {
	    return 0;
    }
    return &_idt[i];
}

int install_ir(uint32_t i, uint16_t flags, uint16_t code_sel, IRQ_HANDLER irq) 
{
    if (i > MAX_IDT_INT)
    {
	    return 0;
    }
    if (!irq)
    {
	    return 0;
    }
    uint64_t uiBase   = (uintptr_t)(&(*irq));
    _idt[i].low_base  = (uint16_t)(uiBase & 0xffff);
    _idt[i].high_base = (uint16_t)((uiBase >> 16) & 0xffff);
    _idt[i].reserved  = 0;
    _idt[i].flags     = (uint8_t)(flags);
    _idt[i].code_sel  = code_sel;
    return 0;
}

void idt_ftoi(int a, IRQ_HANDLER irq)
{
    install_ir(a, IDT_PR|IDT_32, _code_sel, irq);
}

int idt_init(uint16_t code_sel) 
{
    idt_limit = sizeof(idt_desc) * MAX_IDT_INT -1;
    idt_base  = (uint32_t)&_idt[0];
    memset ((void*)&_idt[0], 0, sizeof (idt_desc) * MAX_IDT_INT-1);
    //for (int i=0; i<MAX_IDT_INT; i++)
    //{
    //	install_ir (i, IDT_PR|IDT_32, code_sel, (IRQ_HANDLER)default_handler);
    //}
    _code_sel = code_sel;
    idtsetup();
    idt_install();
    return 0;
}
#pragma GCC pop_options
