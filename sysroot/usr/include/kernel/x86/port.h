/*
 *  port.h
*/

#ifndef _PORT_H_
#define _PORT_H_

#include <stdint.h>

static inline void outb(uint8_t output, uint16_t ioport)
{
	__asm__ volatile(
		"outb %0, %1"  /* src, dest */
		:
		: "r"(output), "Nd"(ioport)	
	);
}

static inline int inb(uint16_t ioport)
{
	
}

#endif
