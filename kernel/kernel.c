/*
 *  kernel.c
 *  Entrypoint for the kernel is  start_kernel() 
 *  execution starts in 32 bit protected mode.
 *
 */

#include <stdint.h>
#include <stddef.h>

#include <kernel/x86/pminit.h>
#include <kernel/x86/keyboard.h>
#include <kernel/vga.h>
#include <kernel/panic.h>
#include <kernel/mm.h>

#include <kernel/klib.h>
#include <kernel/kstring.h>


void start_kernel(int *mbheader, uint32_t mbmagic, uint32_t *heap_top)
{
	init_vga();
	printk("Booting\n");
	/* setup gdt and interrupts */
	pminit();
	/* initialize memory management */
	mminit(mbheader);
	/* setup keyboard */
	kbdinit();

	while(1)
		__asm__( "hlt;");

	/*Some time later... execution should've proceeded to the scheduler.
	PANIC!!*/
	panic("END OF KERNEL CODE");
}
