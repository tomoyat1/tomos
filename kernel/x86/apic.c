/*
 * apic.c
 * everything apic related
 */

#include <kernel/x86/apic.h>

#include <kernel/timer.h>


#include <kernel/x86/port.h>
#include <kernel/x86/interrupts.h>
#include <kernel/x86/pit.h>

#include <kernel/panic.h>
#include <kernel/klib.h>

#include <stdint.h>
#include <stdbool.h>


#define PIC1_CMD 0x20
#define PIC2_CMD 0xa0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xa1

#define SPURIOUS_VECTOR 0xFF
#define APIC_ENABLE (0x1 << 8)

#define APIC_TIME_DIVISOR 0x3

extern bool pit_oneshot_done;

static inline void remap_pic()
{
	outb(0x11, PIC1_CMD); /* ICW1, ICW4 needed*/
	outb(0x11, PIC2_CMD);

	outb(0x20, PIC1_DATA); /* Remap */
	outb(0x28, PIC2_DATA);

	outb(0x4, PIC1_DATA); /* Has slave at irq 2 */
	outb(0x2, PIC2_DATA); /* Slave identity */

	outb(0x89, PIC1_DATA); /* 8086 mode */
	outb(0x89, PIC2_DATA);
}

static inline void mask_pic()
{
	outb(0xfe, PIC1_DATA);
	outb(0xff, PIC2_DATA);
}

static inline void write_ioapic_reg(uint32_t index, uint32_t value)
{
	uint32_t volatile *ioregsel = (uint32_t *)0xBFC00000;
	uint32_t volatile *iowin = (uint32_t *)0xBFC00010;

	*ioregsel = index;
	*iowin = value;
}

static inline uint32_t read_ioapic_reg(uint32_t index)
{
	return 0;
}

static inline void write_lapic_reg(uint32_t index, uint32_t value)
{
	uint32_t *reg = (uint32_t *)(0xBFE00000 + index);
	*reg = value;
}

static inline uint32_t read_lapic_reg(uint32_t index)
{
	uint32_t *reg = (uint32_t *)(0xBFE00000 + index);
	return *reg;
}

void apic_eoi()
{
	write_lapic_reg(0xB0, 0x1);
}

void apic_timer_init()
{
	prime_pit(10);

	/* APIC timer divisor: 16  */
	write_lapic_reg(0x3E0, APIC_TIME_DIVISOR);
	write_lapic_reg(0x380, 0xFFFFFFFF);
	fire_pit();
	while (!is_oneshot_done())
		continue;

	uint32_t ticks_in_10ms = 0xFFFFFFFF - read_lapic_reg(0x390);
	uint32_t timer_count = ticks_in_10ms / 10;
	write_lapic_reg(0x320, 0x20022);
	write_lapic_reg(0x3E0, APIC_TIME_DIVISOR);
	write_lapic_reg(0x380, timer_count);
}

void apic_timer_handler()
{
	inc_sys_clock();
	apic_eoi();
}

void initapic()
{
	/* Remap PIC */
	remap_pic();

	/* Detect APIC, panic if not found */
	int ret = 0;
	__asm__(
	"movl $0x1, %%eax\n\t"
	"cpuid\n\t"
	"movl %%edx, %%ecx\n\t"
	:"=c"(ret)
	:
	: "eax", "edx"
	);
	if (!(ret & (1 << 9)))
		panic("No APIC found.");

	/* Disable (mask) PIC*/
	mask_pic();

	/* Get APIC ID for first CPU */
	uint32_t apic_id = read_lapic_reg(0x20);

	write_ioapic_reg(0x10, 0x20);
	write_ioapic_reg(0x11, (apic_id << 24) & 0x0F000000);

	write_ioapic_reg(0x12, 0x21);
	write_ioapic_reg(0x13, (apic_id << 24) & 0x0F000000);

	/* mask mouse input */
	write_ioapic_reg(0x28, 0x100FF);
	write_ioapic_reg(0x29, (apic_id << 24) & 0x0F000000);

	/* Spurious Interrupt Vector */
	write_lapic_reg(0xF0, SPURIOUS_VECTOR + APIC_ENABLE);
	
	/* enable interrupts */
	__asm__("sti");

	/* APIC timer initialization */
	apic_timer_init();
}
