/*
  term.h contains headers and definitions for terminal handling
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include "string.h"
/* Hardware text mode color constants. */
enum vga_color 
{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};

//vga helper functions, see term.c
uint8_t make_color(enum vga_color fg, enum vga_color bg);
uint16_t make_vgaentry(char c, uint8_t color);

//terminal handling functions, see term.c
void terminal_initialize();
void terminal_clear();
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_writestring(const char* data);
void terminal_movecursor(size_t x, size_t y);
void terminal_adjustcursor(int x, int y);
void terminal_hexstring(void* s, uint32_t n);
