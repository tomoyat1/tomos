/*
 *
 *
 *
 *
 */


#include <kernel/panic.h>

#include <kernel/printk.h>

void panic(char *message)
{
	fill_screen_with_color(COLOR_RED);
	setcolor(COLOR_LIGHT_GREY, COLOR_RED);
	printk("KERNEL PANIC!\nSystem halted.\n");	
	printk(message);
	__asm__("hlt");
}
