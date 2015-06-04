/*
 * panic.c
 * kernel panic
 */


#include <kernel/panic.h>

#include <kernel/klib.h>

void panic(char *message)
{
	fill_screen_with_color(COLOR_RED);
	setcolor(COLOR_LIGHT_GREY, COLOR_RED);
	printk("KERNEL PANIC!\nSystem halted.\n");	
	printk(message);
	printk("\n");
	__asm__(
	"cli;"
	"hlt;"
	);
}
