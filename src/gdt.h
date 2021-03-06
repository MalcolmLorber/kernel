#ifndef _GDT_INC
#define _GDT_INC

#include <stdint.h>

//file to define and use the global descriptor table

#define MAX_GDT_DESC 3
#define GDT_ACC 0x0001
#define GDT_RW  0x0002
#define GDT_DIR 0x0004
#define GDT_EXE	0x0008
#define GDT_CDT 0x0010
#define GDT_DPL	0x0060
#define GDT_MEM	0x0080

#define GDT_GR_HI 0x0f
#define GDT_GR_OS 0x10
#define GDT_GR_32 0x40
#define GDT_GR_PG 0x80

typedef struct //this represents a single entry in the GDT
{
    uint16_t lim;
    uint16_t low_base;
    uint8_t  mid_base;
    uint8_t  flags;
    uint8_t  gran;
    uint8_t  high_base;
} __attribute__((packed)) gdt_desc;

//functions to work with the gdt, see gdt.c for more info
extern void gdt_set_desc(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);
extern gdt_desc* gdt_get_desc(int i);
extern int gdt_init();

#endif
