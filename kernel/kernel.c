/*
 *  kernel.c
 *  Entrypoint for the kernel is  start_kernel() 
 *  execution starts in 32 bit protected mode.
 *
 */

#include <stdint.h>
#include <stddef.h>

#include <kernel/x86/pminit.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/vga.h>
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
	printk("Booting\n");
	for (int i = 0; i < 23; i++) {
		if (i % 10 == 0)
			printk("bar\n");
		/*else if (i % 3 == 1)
			printk("eins\n");*/
		else
			printk("foo\n");
	}

	while(1)
		__asm__( "hlt;");

	/*Some time later... execution should've proceeded to the scheduler.
	PANIC!!*/
	panic("END OF KERNEL CODE");
}
