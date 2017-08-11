#include "gdt.h"
#include "string.h"
#include "serial.h"

#pragma GCC push_options
#pragma GCC optimize ("0")

uint16_t gdt_limit;
uint32_t gdt_base;

//the actual global descriptor table
gdt_desc _gdt [MAX_GDT_DESC];

//install a new gdt
void gdt_install()
{
    struct
    {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) gdtr;
    gdtr.limit = gdt_limit;
    gdtr.base = gdt_base;
    asm volatile ("lgdt (%0)" : : "r" (&gdtr));
    asm volatile ( "ljmp %0, $1337f \n\t 1337: \n\t" :: "i" (8) );
    asm volatile ("mov $0x10, %ax;               \
                 mov %ax, %ds;			\
                 mov %ax, %es;			\
                 mov %ax, %fs;			\
                 mov %ax, %gs;			\
                 mov %ax, %ss;");
}

//set an individual descriptor in the gdt
void gdt_set_desc(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran)
{
    if (i > MAX_GDT_DESC)
    {
        return;
    }
    memset((void*)&_gdt[i], 0, sizeof(gdt_desc));

    _gdt[i].low_base = (uint16_t)(base & 0xffff);
    _gdt[i].mid_base = (uint8_t)((base >> 16) & 0xff);
    _gdt[i].high_base = (uint8_t)((base >> 24) & 0xff);
    _gdt[i].lim = (uint16_t)(limit & 0xffff);

    _gdt[i].flags = access;
    _gdt[i].gran = (uint8_t)((limit >> 16) & 0x0f);
    _gdt[i].gran |= gran & 0xf0;
}

//get a single descriptor from the gdt
gdt_desc* gdt_get_desc(int i)
{
    if (i > MAX_GDT_DESC)
    {
        return 0;
    }
    return &_gdt[i];
}

//initial setup of the gdt at kernel boot
int gdt_init()
{
    gdt_limit = (sizeof(gdt_desc) * MAX_GDT_DESC) - 1;
    gdt_base = (uint32_t)&_gdt[0];
    //null
    gdt_set_desc(0, 0, 0, 0, 0);
    //code
    gdt_set_desc(1, 0, 0xffffffff, GDT_RW | GDT_EXE | GDT_CDT | GDT_MEM, GDT_GR_PG | GDT_GR_32 | GDT_GR_HI);
    //data
    gdt_set_desc(2, 0, 0xffffffff, GDT_RW | GDT_CDT | GDT_MEM, GDT_GR_PG | GDT_GR_32 | GDT_GR_HI);
    //serial_writestring("gdt filled, now installing\n");
    gdt_install();
    return 0;
}
#pragma GCC pop_options
