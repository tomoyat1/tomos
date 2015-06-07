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

uint32_t *mbstruct;

void start_kernel(uint32_t *sysmbstruct, uint32_t mbmagic, uint32_t *heap_top)
{
	mbstruct = sysmbstruct;
	init_vga();
	printk("Booting\n");
	/* setup gdt and interrupts */
	pminit();
	/* initialize memory management */
	mminit(mbstruct);
	/* setup keyboard */
	kbdinit();
	printk("Booting\n");
	for (int i = 0; i < 23; i++) {
		if (i % 10 == 0)
			printk("bar\n");
		else
			printk("foo\n");
	}

	while(1)
		__asm__( "hlt;");

	/*
	 * Some time later... execution should've proceeded to the scheduler.
	 * PANIC!!
	 */
	panic("END OF KERNEL CODE");
}
