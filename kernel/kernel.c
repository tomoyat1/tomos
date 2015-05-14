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

#include <kernel/klib.h>
#include <kernel/kstring.h>
void start_kernel(uint32_t *mbheader, uint32_t mbmagic, uint32_t *heap_top)
{
	init_vga();
	printk("Booting\n");
	//setup gdt and idt
	pminit();
	
	mminit();
	char *ptr_to_char = (char *)kmalloc(sizeof(char[14]));
	kstrcpy(ptr_to_char, "Hello, World\n");
	printk(ptr_to_char);
	char *second_ptr = (char *)kmalloc(sizeof(char[15]));
	kstrcpy(second_ptr, "Hello, World2\n");
	printk(second_ptr);
	__asm__("hlt");
	/*Some time later... execution should've proceeded to the scheduler.
	PANIC!!*/
	panic("END OF KERNEL CODE");
}
