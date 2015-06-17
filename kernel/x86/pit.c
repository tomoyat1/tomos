#include <kernel/x86/pit.h>

#include <kernel/panic.h>
#include <kernel/klib.h>

#include <kernel/x86/port.h>
#include <kernel/x86/interrupts.h>

#include <stdint.h>
#include <stdbool.h>

#define GET_HIGH_BITS(count)\
	((uint8_t)(count & 0xFF00) >> 8)

#define GET_LOW_BITS(count)\
	((uint8_t)count & 0xFF)

static uint32_t count = 0;

bool pit_oneshot_done;

void prime_pit(uint32_t time)
{
	count = 1193 * time;
	if (count >= 65536)
		panic("Count for PIT too large.");

	outb(0x30, 0x43);
	outb(GET_LOW_BITS((uint16_t)count), 0x40);

	pit_oneshot_done = false;
}

void fire_pit()
{
	if (count != 0)
		outb(GET_HIGH_BITS((uint16_t)count), 0x40);
	count = 0;
}

void square_pit(uint32_t time)
{
	count = 1193 * time;
	if (count >= 65536)
		panic("Count for PIT too large.");

	outb(0x36, 0x43);

	outb(GET_LOW_BITS((uint16_t)count), 0x40);

	if (count != 0)
		outb(GET_HIGH_BITS((uint16_t)count), 0x40);

	count = 0;
}
void oneshot_handler()
{
	pit_oneshot_done = true;
	pic_eoi();
}
