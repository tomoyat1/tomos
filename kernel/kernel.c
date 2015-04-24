/*
 *  kernel.c
 *  Entrypoint for the kernel is  start_kernel() 
 *  execution starts in 32 bit protected mode.
 *
 */

#include <kernel/pminit.h>
#include <kernel/mm.h>
#include <kernel/vga.h>
//#include <kernel/kprint.h>

#include <stdint.h>
#include <stddef.h>

uint16_t* buffer = (uint16_t *)0xC00B3000;

void panic()
{
	for ( size_t y = 0; y < 25; y++ )
	{
		for  ( size_t x = 0; y < 80; x++ )
		{
			const size_t index = y * 25 + x;
			buffer[index] = 0x4F20;
		}
	}

	buffer[0] = 0x4F50;
	buffer[1] = 0x4F41;
	buffer[2] = 0x4F4E;
	buffer[3] = 0x4F49;
	buffer[4] = 0x4F43;
}

void start_kernel(uint32_t mbheader, uint32_t mbmagic)
{
	init_vga();
  //kprint("Booting")
  //enable paging
	pminit();

	/*Some time later... execution should've proceeded to the scheduler.
	PANIC!!*/
	panic();
}
