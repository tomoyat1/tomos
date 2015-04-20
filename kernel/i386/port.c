/*
 *   port.c
 *   inb() and outb(), possibly more function relating to io ports.
 *
 *
 */ 

#include <stdint.h>

#include <kernel/port.h>

void outb(uint8_t output, uint16_t port)
{
	__asm__ volatile(
		"outb %0, %1"
		:
		: "a"(output), "Nd"(port)	
	);

}
