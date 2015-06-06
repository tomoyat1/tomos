/*
 * keyboard.c
 * PS/2 keyboard driver
 */
#include <kernel/drivers/keyboard.h>
#include <kernel/klib.h>

#include <kernel/x86/interrupts.h>
#include <kernel/x86/port.h>

void kbdinit()
{
	outb(0x1, 0x60);
	outb(0x60, 0x64);
	outb(0xAE, 0x64);
}

void keypress()
{
	printk("Key Pressed!\n");
	eoi();
}
