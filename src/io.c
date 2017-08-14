#include "io.h"
#include "pic.h"
#include "serial.h"
#include "idt.h"
#include "string.h"
#include "term.h"
#include "comp.h"

//maps for keyboard input to interpret raw scancodes
char scan_map[256] = {'\0','\0','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
		      'q','w','e','r','t','y','u','i','o','p','[',']','\n','\0','a','s',
		      'd','f','g','h','j','k','l',';','\'','`','\0','\\','z','x','c','v',
		      'b','n','m',',','.','/','\0','\0','\0',' ','\0','\0','\0','\0','\0','\0',
		      '\0','\0','\0','\0','\0','\0','\0'};
char shift_map[256] = {'\0','\0','!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
		      'Q','W','E','R','T','Y','U','I','O','P','{','}','\n','\0','A','S',
		      'D','F','G','H','J','K','L',':','\"','~','S','|','Z','X','C','V',
		      'B','N','M','<','>','?','\0','\0','\0',' ','\0','\0','\0','\0','\0','\0',
		      '\0','\0','\0','\0','\0','\0','\0'};
char kb_buf[256];
int kb_buf_size;
int modifiers[6] = {0,0,0};
#define MOD_SHIFT 0
#define MOD_CTRL 1
#define MOD_ALT 2

//helper function to write an int to the terminal
void terminal_writeint(int n)
{
    char f[20];
    itoa(n,f);
    terminal_writestring(f);
}

//processes a terminal command after pressing enter
void io_process(char* s, int ss)
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
    else if(strncmp(s,"clear",5)==0)
    {
	terminal_clear();
    }
    else if(strncmp(s,"hex ",4)==0)
    {
	terminal_hexstring(s+4,ss-4);
	terminal_putchar('\n');
    }
    else if(strncmp(s,"push ",5)==0)
    {
	int n = stoi(s+5);
        cpush(n);
    }
    else if(strncmp(s,"peek",4)==0)
    {
	terminal_writeint(cpeek());
	terminal_putchar('\n');
    }
    else if(strncmp(s,"pop",3)==0)
    {
	terminal_writeint(cpeek());
        cpop();
	terminal_putchar('\n');
    }
    else if(strncmp(s,"add",3)==0)
    {
	cadd();
	terminal_writeint(cpeek());
	terminal_putchar('\n');
    }
    else if(strncmp(s,"mult",3)==0)
    {
	cmult();
	terminal_writeint(cpeek());
	terminal_putchar('\n');
    }
    else if(strncmp(s,"sub",3)==0)
    {
	csub();
	terminal_writeint(cpeek());
	terminal_putchar('\n');
    }
    
    else if(strncmp(s,"rsub",3)==0)
    {
	crsub();
	terminal_writeint(cpeek());
	terminal_putchar('\n');
    }
    else if(strncmp(s,"div",3)==0)
    {
	cdiv();
	terminal_writeint(cpeek());
	terminal_putchar('\n');
    }
    else if(strncmp(s,"rdiv",3)==0)
    {
	crdiv();
	terminal_writeint(cpeek());
	terminal_putchar('\n');
    }
    else 
    {
	terminal_writestring("Command not recognized\n");
    }
}

//interprets keyboard input
//currently directly writes to terminal, should eventually write to an input buffer instead
void kbd_irq()
{
    int a = inb(0x60);
    if(a==0x2a || a==0x36)
    {
	modifiers[MOD_SHIFT]=1;
    }
    else if(a==0xaa || a==0xba)
    {
	modifiers[MOD_SHIFT]=0;
    }
    else if(a==0x1d)
    {
	modifiers[MOD_CTRL]=1;
    }
    else if(a==0x9d)
    {
	modifiers[MOD_CTRL]=0;
    }
    else
    {
	if(a==0x4b)//left arrow
	{
	    terminal_adjustcursor(-1,0);
	}
	else if(a==0x4d)//right arrow
	{
	    terminal_adjustcursor(1,0);
	}
	else if(a==0x48)//up arrow
	{
	    terminal_adjustcursor(0,0);//-1);
	}
	else if(a==0x50)//down arrow
	{
	    terminal_adjustcursor(0,0);//1);
	}
	else if(a<0x80)
	{
	    char in = scan_map[a];
	    if(modifiers[MOD_SHIFT]==1)
	    {
		in = shift_map[a];
	    }
	    if(in == '\n')//enter pressed
	    {
		io_process(kb_buf, kb_buf_size);
		for(int i=0;i<kb_buf_size;i++)
		{
		    kb_buf[i]='\0';
		}
		kb_buf_size=0;
		terminal_writestring("> ");
	    }
	    else if(in=='\b')//backspace pressed
	    {
		if(kb_buf_size>0)
		{
		    kb_buf_size--;
		    kb_buf[kb_buf_size]='\0';
		    terminal_putchar(in);
		}
	    }
	    else if(modifiers[MOD_CTRL]==1&&(in=='l'||in=='L'))//control l to clear 
	    {
		terminal_clear();
		for(int i=0;i<kb_buf_size;i++)
		{
		    kb_buf[i]='\0';
		}
		kb_buf_size=0;
		terminal_writestring("> ");
	    }
	    else if(in!='\0')//base case, we put the typed character into the terminal
	    {
		kb_buf[kb_buf_size]=in;
		kb_buf_size++;
		terminal_putchar(in);  
	    }
	}
    }
    //else
    //terminal_putchar(scan_map[a-0x80]);
    //serial_writestring("\n");
    pic_command(0, PIC_OCW2_EOI);//signal to the pic that we processed the interrupt
}

//installs our keyboard interrupt handler to the interrupt handler
void io_init()
{
    install_c_ir(33, kbd_irq);
}
