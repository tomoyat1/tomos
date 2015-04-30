/*
 *  kernel.c
 *  Entrypoint for the kernel is  start_kernel() 
 *  execution starts in 32 bit protected mode.
 *
 */

#include <stdint.h>
#include <stddef.h>

#include <kernel/pminit.h>
#include <kernel/vga.h>
#include <kernel/printk.h>
#include <kernel/panic.h>
#include <kernel/mm.h>

void start_kernel(uint32_t *mbheader, uint32_t mbmagic, uint32_t *heap_top)
{
	init_vga();
	printk("Booting\n");
	//setup gdt and idt
	pminit();
	
	mminit();
	search_free_mem(1);
	/*Some time later... execution should've proceeded to the scheduler.
	PANIC!!*/
	panic("END OF KERNEL CODE");
}
