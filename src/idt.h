#ifndef _IDT_INC
#define _IDT_INC

#include <stdint.h>

#define MAX_IDT_INT 256

#define IDT_16 0x06
#define IDT_32 0x0E
#define IDT_R1 0x40
#define IDT_R2 0x20
#define IDT_R3 0x60
#define IDT_PR 0x80

typedef void (*IRQ_HANDLER)(void);

typedef struct {
	uint16_t low_base;
	uint16_t code_sel;
	uint8_t	 reserved;
	uint8_t	 flags;
	uint16_t high_base;
} idt_desc;

extern idt_desc* get_ir (uint32_t i);
extern int install_ir (uint32_t i, uint16_t flags, uint16_t code_sel, IRQ_HANDLER);
extern int idt_init (uint16_t code_sel);

#endif
