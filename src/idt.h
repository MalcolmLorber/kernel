#ifndef _IDT_INC
#define _IDT_INC

#include "PCB.h"
#include <stdint.h>

#define MAX_IDT_INT 256

#define IDT_16 0x06
#define IDT_32 0x0E
#define IDT_R1 0x40
#define IDT_R2 0x20
#define IDT_R3 0x60
#define IDT_PR 0x80

typedef void (*IRQ_HANDLER)(void);

//interrupt descriptor table entry
typedef struct 
{
    uint16_t low_base;
    uint16_t code_sel;
    uint8_t  reserved;
    uint8_t  flags;
    uint16_t high_base;
}__attribute__((packed)) idt_desc;


/*struct reg_state
{
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    }__attribute__((packed));*/

 /*struct stack_state
   {
    uint32_t eip;
    uint32_t cd;
    uint32_t eflags;
    }__attribute__((packed));*/

//interrupt handling functions, see idt.c    
extern void default_handler(trapframe reg); 
extern idt_desc* get_ir(uint32_t i);
extern int install_ir(uint32_t i, uint16_t flags, uint16_t code_sel, IRQ_HANDLER);
extern void install_c_ir(uint32_t interrupt, void (*handler) (uint32_t));
extern int idt_init(uint16_t code_sel);
extern void idt_ftoi(int a, IRQ_HANDLER irq);
extern void idtsetup();
#endif
