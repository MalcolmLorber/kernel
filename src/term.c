/*
  term.c contains functions for working with/writing to the terminal

  It was taken from the simple kernel from the osdev wiki
*/

#include "term.h"
#include "asm.h"

//create a vga color code from the two vga color constants
uint8_t make_color(enum vga_color fg, enum vga_color bg) 
{
    return fg | bg << 4;
}

//create a proper character for vga using a character and a color code
uint16_t make_vgaentry(char c, uint8_t color) 
{
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | color16 << 8;
}

//terminal size, const for now
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

//keeping track of our cursor
size_t terminal_row;
size_t terminal_column;
//current color scheme
uint8_t terminal_color;
//buffer for holding current terminal state
uint16_t* terminal_buffer;

//changes the location of the cursor on screen
void update_csr()
{
    unsigned temp = terminal_row*80+terminal_column;
    outb(0x3D4,14);
    outb(0x3D5,temp>>8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}

//scrolls our terminal by one line
void terminal_scroll()
{
    for(size_t y=0;y<VGA_HEIGHT-1;y++)
    {
	for(size_t x=0;x<VGA_WIDTH;x++)
	{
	    terminal_buffer[y*VGA_WIDTH + x]=terminal_buffer[(y+1)*VGA_WIDTH + x];
	}
    }
    for(size_t x=0;x<VGA_WIDTH;x++)
    {
	terminal_buffer[(VGA_HEIGHT-1)*VGA_WIDTH + x]=make_vgaentry(' ', terminal_color);
    }
}

//clears the terminal
void terminal_clear()
{
    for(size_t y=0;y<VGA_HEIGHT;y++)
    {
	for(size_t x=0;x<VGA_WIDTH;x++)
	{
	    terminal_buffer[y*VGA_WIDTH + x]=make_vgaentry(' ', terminal_color);;
	}
    }
    terminal_row = terminal_column = 0;
}

//setup of terminal
void terminal_initialize() 
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) 
    {
	for (size_t x = 0; x < VGA_WIDTH; x++) 
	{
	    const size_t index = y * VGA_WIDTH + x;
	    terminal_buffer[index] = make_vgaentry(' ', terminal_color);
	}
    }
}

//change terminal color, only applies to future updates
void terminal_setcolor(uint8_t color) 
{
    terminal_color = color;
}

//add a character anywhere on screen
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vgaentry(c, color);
}

//add a character at the cursor and advance the cursor
void terminal_putchar(char c) 
{
    if(c=='\b')
    {
	if(terminal_column == 0)
	{
	    terminal_row--;
	    terminal_column=VGA_WIDTH;
	}
	terminal_column--;
	terminal_putentryat(' ',terminal_color, terminal_column,terminal_row);
    }
    else if(c=='\n')
    {
	terminal_column=0;
	terminal_row++;
	if(terminal_row>=VGA_HEIGHT)
	{
	    terminal_scroll();
	    terminal_row=VGA_HEIGHT-1;
	}
    }
    else if(c=='\t')
    {
	terminal_writestring("    ");
    }
    else
    {
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) 
	{
	    terminal_column = 0;
	    if (++terminal_row == VGA_HEIGHT) 
	    {
		//terminal_row = 0;
		terminal_scroll();
		terminal_row=VGA_HEIGHT-1;
	    }
	}
    }
    update_csr();
}

//allow printing of hexadecimal numbers cleanly
const char* hexcharss = "0123456789abcdef";
void terminal_hexstring(void* s, uint32_t n) 
{
    char* d = (char*)s;
    for (uint32_t i = 0; i < n; i++)
    {
        terminal_putchar(hexcharss[(d[i]>>4)&0xf]);
        terminal_putchar(hexcharss[d[i]&0xf]);
    }
}

//write a whole string at the cursor
void terminal_writestring(const char* data) 
{
    size_t datalen = strlen(data);
    for (size_t i = 0; i < datalen; i++)
    {
	terminal_putchar(data[i]);
    }
}

//move the cursor to a given position
void terminal_movecursor(size_t x, size_t y)
{
    if(x>=VGA_WIDTH || y>= VGA_HEIGHT) return;
    terminal_column=x;
    terminal_row=y;
    update_csr();
}

//offset the cursor from its current position by a given amount
void terminal_adjustcursor(int x, int y)
{
    terminal_column+=x;
    terminal_row+=y;
    if(terminal_column>VGA_WIDTH) terminal_column=VGA_WIDTH;
    if(terminal_row>VGA_HEIGHT) terminal_row=VGA_HEIGHT;
    update_csr();
}
