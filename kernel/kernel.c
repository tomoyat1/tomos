/*
 *  kernel.c
 *  Entrypoint for the kernel is  start_kernel() 
 *  execution starts in 32 bit protected mode.
 *
 */

#include <kernel/pminit.h>
#include <kernel/vga.h>
//#include <kernel/kprint.h>

#include <stdint.h>
#include <stddef.h>

uint16_t* buffer = (uint16_t *)0xC00B8000;

void panic()
{
	for ( size_t y = 0; y < 25; y++ )
	{
		for  ( size_t x = 0; x < 80; x++ )
		{
			const size_t index = y * 25 + x;
			buffer[index] = 0x4F20;
		}
	}
}
void start_kernel(uint32_t mbheader, uint32_t mbmagic)
{
	init_vga();
	//kprint("Booting")
	//init GDT 
	//TODO: DO THIS BEFORE CR0.PE = 1
	pminit();
	//Do page table setup in C as well
	/*Some time later... execution should've proceeded to the scheduler.
	PANIC!!*/
	panic();
}
