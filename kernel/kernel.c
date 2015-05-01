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
#include <kernel/panic.h>
#include <kernel/mm.h>

#include <kernel/kmalloc.h>
#include <kernel/printk.h>
void start_kernel(uint32_t *mbheader, uint32_t mbmagic, uint32_t *heap_top)
{
	init_vga();
	printk("Booting\n");
	//setup gdt and idt
	pminit();
	
	mminit();
	char *ptr_to_char = (char *)kmalloc(sizeof(char[13]));
	ptr_to_char = "Hello, World\0";
	printk(ptr_to_char);
	/*Some time later... execution should've proceeded to the scheduler.
	PANIC!!*/
	panic("END OF KERNEL CODE");
}
