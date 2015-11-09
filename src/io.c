#include "io.h"
#include "pic.h"
#include "serial.h"
#include "idt.h"
#include "string.h"
#include "term.h"

char scan_map[256] = {' ','E','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p',
		      '[',']','\n','^','a','s','d','f','g','h','j','k','l',';','\'','`','S','\\','z','x','c','v','b','n','m',
		      ',','.','/','S','O','A',' ','C','F','F','F','F','F','F','F','F','F','F','L','L',};
char kb_buf[256];
int kb_buf_size;
int cstack[128];
int cstack_size;

void io_process(char* s)
{
    terminal_putchar('\n');
    if(strncmp(s,"echo ",5)==0)
    {
	terminal_writestring(s+5);
	terminal_putchar('\n');
    }
    else if(strncmp(s,"serial ",7)==0)
    {
	serial_writestring("\n");
	serial_writestring(s+7);
	serial_writestring("\n");
    }
    else if(strncmp(s,"hex ",4)==0)
    {
	terminal_hexstring(s+4,16);
	terminal_putchar('\n');
    }
    else if(strncmp(s,"push ",5)==0)
    {
	int n = stoi(s+5);
	cstack[cstack_size]=n;
	cstack_size++;
	//terminal_writestring("pushed\n");
    }
    else if(strncmp(s,"pop",3)==0)
    {
	if(cstack_size==0)
	{
	    terminal_writestring("stack empty\n");
	}
	else
	{
	    terminal_putchar(cstack[cstack_size-1]+48);
	    cstack_size--;
	    terminal_putchar('\n');
	}
    }
}

void kbd_irq()
{
    int a = inb(0x60);
    //serial_hexword(a);
    if(a==0x4b)
	terminal_adjustcursor(-1,0);
    else if(a==0x4d)
	terminal_adjustcursor(1,0);
    else if(a==0x48)
	terminal_adjustcursor(0,-1);
    else if(a==0x50)
	terminal_adjustcursor(0,1);
    else if(a<0x80)
    {
	char in = scan_map[a];
	if(in == '\n')//enter pressed
	{
	    io_process(kb_buf);
	    for(int i=0;i<kb_buf_size;i++)
	    {
		kb_buf[i]='\0';
	    }
	    kb_buf_size=0;
	}
	else if(in=='\b')
	{
	    kb_buf_size--;
	    kb_buf[kb_buf_size]='\0';
	    terminal_putchar(in);
	}
	else
	{
	    kb_buf[kb_buf_size]=in;
	    kb_buf_size++;
	    terminal_putchar(in);  
	}
    }
    //else
    //terminal_putchar(scan_map[a-0x80]);
    //serial_writestring("\n");
    pic_command(0, PIC_OCW2_EOI);
}

void io_init()
{
    install_c_ir(33, kbd_irq);
}
