#include "pit.h"
#include "pic.h"
#include "idt.h"

static volatile uint32_t _pit_ticks = 0;
static bool _pit_is_init = false;

void pit_irq()
{
    asm volatile ("add %esp, 12; pushad");
    _pit_ticks++;
    pic_command(PIC_OCW2_EOI, 0);
    asm volatile("popad; iretd");
}

void pit_command(uint8_t cmd)
{
    outb(PIC_REG_COMD, cmd);
}

void pit_write(uint16_t data, uint8_t counter)
{
    uint16_t port= (counter == PIT_OCW_CONT_0) ? PIT_REG_CONT_0 :
	((counter == PIT_OCW_CONT_1) ? PIT_REG_CONT_1 : PIT_REG_CONT_2);
    outb(port, (uint8_t)data);
}

uint8_t pit_read(uint16_t counter)
{
    uint16_t port= (counter == PIT_OCW_CONT_0) ? PIT_REG_CONT_0 :
	((counter == PIT_OCW_CONT_1) ? PIT_REG_CONT_1 : PIT_REG_CONT_2);
    return inb(port);
}

uint32_t pit_set_tick(uint32_t i)
{
    uint32_t ret = _pit_ticks;
    _pit_ticks = i;
    return ret;
}

uint32_t pit_get_tick()
{
    return _pit_ticks;
}

void pit_start_counter(uint32_t frequency, uint8_t counter, uint8_t mode)
{
    if(freq == 0)
    {
	return;
    }

    uint16_t div = (uint16_t)(1193181 / (uint16_t)freq);

    uint8_t ocw = 0;
    ocw = (ocw & ~PIT_OCW_MODE) | mode;
    ocw = (ocw & ~PIT_OCW_RL) | PIT_OCW_RL_DATA;
    ocw = (ocw & ~PIT_OCW_CONT) | counter;
    pit_command(ocw);

    pit_write(div & 0xFF, 0);
    pit_write((div >> 8)& 0xFF, 0);
    
    _pit_ticks = 0;
}

void pit_init();
{
    install_ir(32, IDT_PR|IDT_32, 0x8, pit_irq);
    _pit_is_init = true;
}

bool pit_is_init()
{
    return _pit_is_init;
}
