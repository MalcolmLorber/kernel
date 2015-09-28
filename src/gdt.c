#include "gdt.h"
#include "string.h"
typedef struct{
  uint16_t limit;
  uint32_t base;
}gdtr;

static gdt_desc _gdt [MAX_GDT_DESC];
static gdtr _gdtr;

static void gdt_install () {
  //TODO: VOODOO MAGIC
}

void gdt_set_desc(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran)
{
  if (i > MAX_GDT_DESC)
    return;
  memset ((void*)&_gdt[i], 0, sizeof (gdt_desc));
  
  _gdt[i].low_base  = (uint16_t)(base & 0xffff);
  _gdt[i].mid_base  = (uint8_t)((base >> 16) & 0xff);
  _gdt[i].high_base = (uint8_t)((base >> 24) & 0xff);
  _gdt[i].lim	    = (uint16_t)(limit & 0xffff);
  
  //! set flags and grandularity bytes
  _gdt[i].flags = access;
  _gdt[i].gran = (uint8_t)((limit >> 16) & 0x0f);
  _gdt[i].gran |= gran & 0xf0;
}
gdt_desc* gdt_get_desc (int i) {

  if (i > MAX_GDT_DESC)
    return 0;
  return &_gdt[i];
}
int i86_gdt_initialize () {
  _gdtr.limit = (sizeof (gdt_desc) * MAX_GDT_DESC)-1;
  _gdtr.base = (uint32_t)&_gdt[0];
  //null
  gdt_set_desc(0, 0, 0, 0, 0);
  //code
  gdt_set_desc(1,0,0xffffffff, GDT_RW|GDT_EXE|GDT_CDT|GDT_MEM, GDT_GR_PG|GDT_GR_32|GDT_GR_HI);
  //data
  gdt_set_desc(2,0,0xffffffff, GDT_RW|GDT_CDT|GDT_MEM, GDT_GR_PG|GDT_GR_32|GDT_GR_HI);
  gdt_install();  
  return 0;
}
