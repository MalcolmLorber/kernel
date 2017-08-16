#include "pit.h"
#include "pic.h"
#include "idt.h"
#include "asm.h"
#include "serial.h"

//without these options GCC optimizes out many of the calls to the pit and pic
#pragma GCC push_options
#pragma GCC optimize ("0")

static volatile uint32_t _pit_ticks = 0;
static bool _pit_is_init = false;

//this is called whenever the PIT "ticks"
void pit_irq()
{
    //asm volatile ("pusha");
    _pit_ticks++;
    //acknowledge the tick
    pic_command(0, PIC_OCW2_EOI);
    //asm volatile("nop;nop;nop;popa; iret;");
}

//send a command to the PIT
void pit_command(uint8_t cmd)
{
    outb(PIT_REG_COMD, cmd);
}

//write data to the PIT
void pit_write(uint16_t data, uint8_t counter)
{
    uint16_t port= (counter == PIT_OCW_CONT_0) ? PIT_REG_CONT_0 :
	((counter == PIT_OCW_CONT_1) ? PIT_REG_CONT_1 : PIT_REG_CONT_2);
    outb(port, (uint8_t)data);
}

//read data from the PIT
uint8_t pit_read(uint16_t counter)
{
    uint16_t port= (counter == PIT_OCW_CONT_0) ? PIT_REG_CONT_0 :
	((counter == PIT_OCW_CONT_1) ? PIT_REG_CONT_1 : PIT_REG_CONT_2);
    return inb(port);
}

//set the tick count
uint32_t pit_set_tick(uint32_t i)
{
    uint32_t ret = _pit_ticks;
    _pit_ticks = i;
    return ret;
}

//get how many times the PIT has ticked
uint32_t pit_get_tick()
{
    return _pit_ticks;
}

//allows us to keep a counter based off of the tick, at whatever frequency we want
void pit_start_counter(uint32_t freq, uint8_t counter, uint8_t mode)
{
    if(freq == 0)
    {
	return;
    }

    //magic number to get the proper frequency of ticks for the PIT
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

//initializes the pit by adding it to our interrupt handler table
void pit_init()
{
    //install_ir(32, IDT_PR|IDT_32, 0x8, pit_irq);
    install_c_ir(32, pit_irq);
    _pit_is_init = true;
}

//simple function to check if the initialization of the PIT has finished
bool pit_is_init()
{
    return _pit_is_init;
}
#pragma GCC pop_options
