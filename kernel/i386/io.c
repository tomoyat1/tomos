#include <kernel/io.h>
#include <stddef.h>
#include <stdint.h>

void outb(uint16_t ioport, uint8_t output)
{
	__asm__ __volatile__(
		"outb %1, %2"
		:
		:"r"(ioport), "r"(output)	
	);

}
