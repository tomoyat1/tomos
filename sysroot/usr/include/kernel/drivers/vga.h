#ifndef _VGA_H_
#define _VGA_H_

/*   vga.h
 *
 *   fill_screen_with_color() will reset cursor position to (0,0)
 */

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define MAKE_COLOR(fg,bg) \
(fg | (bg<<4) )

#define MAKE_ENTRY(c,color) \
(((uint16_t)color<<8)|c)

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

void init_vga();

void putentryat(uint16_t entry, size_t x, size_t y);
void write_char(char c);
void setcolor(enum vga_color fg, enum vga_color bg);
void newline();
void fill_screen_with_color(enum vga_color bg);

#endif
