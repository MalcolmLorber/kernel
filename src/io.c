#include "io.h"
#include "pic.h"
#include "serial.h"
#include "idt.h"

char scan_map[256] = {' ','E','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p',
		      '[',']','\n','^','a','s','d','f','g','h','j','k','l',';','\'','`','S','\\','z','x','c','v','b','n','m',
		      ',','.','/','S','O','A',' ','C','F','F','F','F','F','F','F','F','F','F','L','L',};

void kbd_irq()
{
    int a = inb(0x60);
    serial_hexword(a);
    if(a==0x4b)
	terminal_adjustcursor(-1,0);
    else if(a==0x4d)
	terminal_adjustcursor(1,0);
    else if(a==0x48)
	terminal_adjustcursor(0,-1);
    else if(a==0x50)
	terminal_adjustcursor(0,1);
    else if(a<0x80)
	terminal_putchar(scan_map[a]);  
    //else
    //terminal_putchar(scan_map[a-0x80]);
    serial_writestring("\n");
    pic_command(0, PIC_OCW2_EOI);
}

void io_init()
{
    install_c_ir(33, kbd_irq);
}
