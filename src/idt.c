#include "idt.h"
#include "string.h"
#include "serial.h"
#include "exceptions.h"

#pragma GCC push_options
#pragma GCC optimize ("0")

#define serial_val(i) itoa(i,f);serial_writestring(f);serial_writestring(" ");
uint16_t idt_limit;
uint32_t idt_base;

idt_desc _idt [MAX_IDT_INT];

uint16_t _code_sel;

void (*handlers[256]) (uint32_t error);

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

void default_handler(struct reg_state reg, uint32_t interrupt, uint32_t error, struct stack_state stack) 
{
    /*for(uint32_t i=stack.eip-16;i<=stack.eip+16;i+=4)
    {
	serial_hexword(i);
	serial_writestring(": ");
	serial_hexword(*((uint32_t*)(void*)i));
	serial_writestring("\n");
	}*/
    if(interrupt<32)
	stack.eip+=2;
    //serial_writestring("int caught ");
    //char f[20];
    //serial_val(interrupt);
    //serial_hexword(error);serial_writestring(" ");
    //serial_hexword(stack.eip);serial_writestring(" ");
    //serial_val(stack.cd);
    //serial_val(stack.eflags);
    //serial_writestring("\n");
    if(handlers[interrupt]!=NULL)
    {
	handlers[interrupt](error);
    }
    //asm("hlt");
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
void install_c_ir(uint32_t interrupt, void (*handler) (uint32_t))
{
    handlers[interrupt]=handler;
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
    for (int i=0; i<MAX_IDT_INT; i++)
    {
    //	install_ir (i, IDT_PR|IDT_32, code_sel, (IRQ_HANDLER)default_handler);
	handlers[i]=NULL;
    }
    _code_sel = code_sel;
    idtsetup();
    idt_install();
    load_error_handlers();
    return 0;
}
#pragma GCC pop_options
