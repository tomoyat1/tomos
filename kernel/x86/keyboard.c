#include <kernel/x86/keyboard.h>

#include <kernel/x86/port.h>

void kbdinit()
{
	outb(0x1, 0x60);
	outb(0x60, 0x64);
	outb(0xAE, 0x64);
}
