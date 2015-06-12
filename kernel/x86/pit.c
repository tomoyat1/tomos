#include <kernel/x86/pit.h>

#include <kernel/panic.h>
#include <kernel/klib.h>

#include <kernel/x86/port.h>
#include <kernel/x86/interrupts.h>

#include <stdint.h>

#define GET_HIGH_BITS(count)\
	((uint8_t)(count & 0xFF00) >> 8)

#define GET_LOW_BITS(count)\
	((uint8_t)count & 0xFF)
static uint32_t count = 0;

void prime_pit(uint32_t time)
{
	count = 1193 * time;
	if (count >= 65536)
		panic("Count for PIT too large.");

	outb(0x30, 0x43);

	outb(GET_HIGH_BITS((uint16_t)count), 0x40);
}

void fire_pit()
{
	outb(GET_LOW_BITS((uint16_t)count), 0x40);
	count = 0;
}

void pit_handler()
{
	prime_pit(10);
	pic_eoi();
	fire_pit();
}
