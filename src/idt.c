#include "idt.h"
#include <string.h>
typedef struct{
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idtr;

static idt_desc _idt [MAX_IDT_INT];
static idtr _idtr;

static void idt_install () {
  //TODO: VOODOO MAGIC
  asm volatile ("lidt [%0]": : "p"(&_idtr));
}
static void default_handler () {
  //DEBUG OUTPUT MAYBE?
  for(;;);
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
  uint64_t uiBase = (uint64_t)&(*irq);
  _idt[i].low_base  = (uint16_t)(uiBase & 0xffff);
  _idt[i].high_base = (uint16_t)((uiBase >> 16) & 0xffff);
  _idt[i].reserved  = 0;
  _idt[i].flags	  = (uint8_t)(flags);
  _idt[i].code_sel  = code_sel;
  return 0;
}
int idt_init (uint16_t code_sel) {
  _idtr.limit = sizeof (idt_desc) * MAX_IDT_INT -1;
  _idtr.base  = (uint32_t)&_idt[0];
  memset ((void*)&_idt[0], 0, sizeof (idt_desc) * MAX_IDT_INT-1);
  for (int i=0; i<MAX_IDT_INT; i++)
    install_ir (i, IDT_PR|IDT_32,code_sel,(IRQ_HANDLER)default_handler);
  idt_install ();
  return 0;
}
