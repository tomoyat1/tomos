/*
 *  kernel.c
 *  Entrypoint for the kernel is  start_kernel() 
 *  execution starts in 32 bit protected mode.
 *
 */

#include <stdint.h>
#include <stddef.h>

#include <kernel/x86/pminit.h>
#include <kernel/timer.h>
#include <kernel/x86/pit.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/vga.h>
#include <kernel/panic.h>
#include <kernel/mm.h>
#include <kernel/proc.h>
#include <kernel/sched.h>

#include <kernel/klib.h>
#include <kernel/kstring.h>

extern void *kernel_stack_bottom;
extern struct proc_struct *current;

uint32_t *mbstruct;

struct proc_struct *kernel_thread = (struct proc_struct *)&kernel_stack_bottom;

void start_kernel(uint32_t *sysmbstruct, uint32_t mbmagic)
{
	mbstruct = sysmbstruct;
	init_vga();
	printk("Booting\n");

	kernel_thread->next = NULL;
	/* setup gdt and interrupts */
	pminit();
	/* initialize memory management */
	mminit();

	/* setup keyboard */
	kbdinit();

	current = kernel_thread;

	//square_pit(1);
	 while (1) {
		wait(1000);
		printk("Timer ended\n");
	}

	while(1)
		__asm__( "hlt;");

	/*
	 * Some time later... execution should've proceeded to the scheduler.
	 * PANIC!!
	 */
	panic("END OF KERNEL CODE");
}
