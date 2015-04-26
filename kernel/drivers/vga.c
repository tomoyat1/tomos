/*
 *
 *  vga.c
 *  VGA stuff
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <kernel/vga.h>

typedef struct 
{
	volatile size_t row;
	volatile size_t column;
	volatile uint8_t color;
	uint16_t* buffer;
} terminal_t;

terminal_t terminal;

void putentryat(uint16_t entry, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal.buffer[index] = entry;	
}

void setcolor(enum vga_color fg, enum vga_color bg)
{
	terminal.color = MAKE_COLOR(fg,bg);
}

void init_vga()
{
	terminal.row = 0;
	terminal.column = 0;
	setcolor(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal.buffer = (uint16_t*)0xC00B8000;

	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
		const size_t index = y * VGA_WIDTH + x;
		terminal.buffer[index] = MAKE_ENTRY(' ', terminal.color);	
		}
	}
}

void fill_screen_with_color(enum vga_color bg) 
{
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
		const size_t index = y * VGA_WIDTH + x;
		terminal.buffer[index] = MAKE_ENTRY(' ', MAKE_COLOR(0,bg));	
		}
	}
}

