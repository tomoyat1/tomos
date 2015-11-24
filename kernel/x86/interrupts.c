/*
 * interrupts. IDTs, TSS for handling interrupts...
 */

#include <stdint.h>

#include <kernel/x86/interrupts.h>
#include <kernel/x86/port.h>
#include <kernel/x86/apic.h>
#include <kernel/x86/pit.h>

#include <kernel/panic.h>
#include <kernel/klib.h>

#define INTERRUPT(selector, offset, flags)\
 (((selector & (uint64_t )0x0000FFFF) << 16) |\
 ((flags & (uint64_t )0x0000000F) << 45) |\
 ((uint64_t)0xE00 << 32)| /* bits 5 to 12 of second longword */\
 ((offset & (uint64_t )0xFFFF0000) << 32) |\
 ((offset & (uint64_t )0x0000FFFF)))

extern void *kernel_stack_bottom;
extern void *divided_by_zero;
extern void *kbdirq;
extern void *pitirq;
extern void *apic_timer_irq;

struct tss_struct tss;

struct idt_ptr
{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

void set_tss()
{
	tss.ss0 = 0x10;
	tss.esp0 = (uint32_t)&kernel_stack_bottom;
	
	__asm__(
	"movw $0x28, %%cx;"
	"ltr %%cx;"
	:
	:
	:"cx"
	);
}

void set_idt()
{
	static uint64_t idt[0xFF] __attribute__((aligned(8))) =
	{
		[0] = 0
	};
	idt[0] = INTERRUPT(0x8, (uint32_t)&divided_by_zero, 0x4);
	idt[0xd] = INTERRUPT(0x8, (uint32_t)&double_fault_panic, 0x4);
	idt[0x20] = INTERRUPT(0x8, (uint32_t)&pitirq, 0x4);
	idt[0x21] = INTERRUPT(0x8, (uint32_t)&kbdirq, 0x4);
	idt[0x22] = INTERRUPT(0x8, (uint32_t)&apic_timer_irq, 0x4);
	static struct idt_ptr sysidt;
	sysidt.base = (uint32_t)&idt;
	sysidt.limit = (uint16_t)sizeof(idt);

	__asm__(
	"lidtl %0;"
	:
	:"m"(sysidt)
	);
}

void divzero()
{
	panic("divided by zero");
}

void double_fault_panic()
{
	__asm__("cli;hlt");;
}
