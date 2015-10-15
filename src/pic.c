#include "pic.h"
#include "asm.h"

#pragma GCC push_options
#pragma GCC optimize ("0")

uint8_t pic_read(uint16_t pic)
{
    if(pic>1)
    {
	return 0;
    }
    uint16_t reg = (pic==0)? PIC1_REG_DATA:PIC2_REG_DATA;
    return inb(reg);
}

void pic_write(uint16_t pic, uint8_t data)
{
    if(pic>1)
    {
	return;
    }
    uint16_t reg = (pic==0)? PIC1_REG_DATA:PIC2_REG_DATA;
    outb(reg, data);
}

void pic_command(uint16_t pic, uint8_t cmd)
{
    if(pic>1)
    {
	return;
    }
    uint16_t reg = (pic==0)? PIC1_REG_COMD:PIC2_REG_COMD;
    outb(reg, cmd);
}

//void pic_mask_irq(uint16_t pic, uint8_t irqmask)
//{
//    //TODO
//}

void pic_init(uint8_t base0, uint8_t base1)
{
    uint8_t icw=0;

    //enab,e init and expect mode on both
    icw = (icw & ~PIC_ICW1_INIT) | PIC_ICW1_INIT_YES;
    icw = (icw & ~PIC_ICW1_IC4)  | PIC_ICW1_IC4_EXPECT;
    pic_command(0, icw);
    pic_command(1, icw);
    
    //send ICW2 (base irq address)
    pic_write(0, base0);
    pic_write(1, base1);

    //now for magic flags to make the PICs connect
    pic_write(0,0x04);
    pic_write(1,0x02);

    //enable x86 mode on PICs
    icw = (icw & ~PIC_ICW4_UPM) | PIC_ICW4_UPM_86;
    pic_write(0, icw);
    pic_write(1, icw);
}
#pragma GCC pop_options
