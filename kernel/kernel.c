/*
 *  kernel.c
 *  Entrypoint for the kernel is  start_kernel() 
 *  execution starts in 32 bit protected mode.
 *
 */

#include <kernel/pminit.h>
#include <kernel/vga.h>
#include <kernel/printk.h>

#include <stdint.h>
#include <stddef.h>

uint16_t* buffer = (uint16_t *)0xC00B8000;

void panic()
{
	fill_screen_with_color(COLOR_RED);
	setcolor(COLOR_LIGHT_GREY, COLOR_RED);
	printk("KERNEL PANIC!\nREBOOT!!");	
}
void start_kernel(uint32_t mbheader, uint32_t mbmagic)
{
	init_vga();
	printk("Booting");
	pminit();
	//Do page table setup in C as well
	/*Some time later... execution should've proceeded to the scheduler.
	PANIC!!*/
	panic();
}
