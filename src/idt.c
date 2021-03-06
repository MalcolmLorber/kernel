#include "idt.h"
#include "string.h"
#include "serial.h"
#include "exceptions.h"
#include "PCB.h"

#pragma GCC push_options
#pragma GCC optimize ("0")

#define serial_val(i) itoa(i,f);serial_writestring(f);serial_writestring(" ");
uint16_t idt_limit;
uint32_t idt_base;

//the actual interrrupt descriptor table
idt_desc _idt [MAX_IDT_INT];

uint16_t _code_sel;

void (*handlers[256]) (uint32_t error);

//installs a new idt, we just use the one
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

//all the possible error codes in x86, functions defined in exceptions.h and exceptions.c
void load_error_handlers()
{
    void(*errors[])(uint32_t)={int_div_by_zero,int_debug,int_non_maskable_interrupt,int_breakpoint,int_overflow,
                               int_bound_range_exceeded,int_invalid_opcode,int_device_not_available,int_double_fault,
                               int_coprocessor_segment_overrun,int_invalid_tss,int_segment_not_present,
                               int_stack_segment_fault,int_general_protection_fault,int_page_fault,int_reserved,
                               int_x87_floating_point,int_alignment_check,int_machine_check,int_simd_floating_point,
                               int_virtualization,int_reserved,int_reserved,int_reserved,int_reserved,int_reserved,
                               int_reserved,int_reserved,int_reserved,int_reserved,int_security,int_reserved};
    for(int i=0;i<32;i++)
    {
	handlers[i]=errors[i];
    }
}

//default handler for all interrupts, called from idta.asm
void default_handler(trapframe reg) 
{
    settf(&reg);
    if(reg.trap<32)
	reg.eip+=2;
    if(handlers[reg.trap]!=NULL)
    {
	handlers[reg.trap](reg.err);
    }
    return;
}

//returns an idt entry
idt_desc* get_ir(uint32_t i) 
{
    if (i > MAX_IDT_INT)
    {
	    return 0;
    }
    return &_idt[i];
}

//adds an idt entry
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

//adds a c function to the interrupt handlers
void install_c_ir(uint32_t interrupt, void (*handler) (uint32_t))
{
    handlers[interrupt]=handler;
}

//wrapper for install_ir for common case
void idt_ftoi(int a, IRQ_HANDLER irq)
{
    install_ir(a, IDT_PR|IDT_32, _code_sel, irq);
}

//initialization and setup of the idt
int idt_init(uint16_t code_sel) 
{
    idt_limit = sizeof(idt_desc) * MAX_IDT_INT -1;
    idt_base  = (uint32_t)&_idt[0];
    memset ((void*)&_idt[0], 0, sizeof (idt_desc) * MAX_IDT_INT-1);
    for (int i=0; i<MAX_IDT_INT; i++)
    {
    //	install_ir (i, IDT_PR|IDT_32, code_sel, (IRQ_HANDLER)default_handler);
	handlers[i]=NULL;
    }
    _code_sel = code_sel;
    idtsetup();
    idt_install();
    load_error_handlers();
    handlers[0x80] = syscall;
    return 0;
}
#pragma GCC pop_options
