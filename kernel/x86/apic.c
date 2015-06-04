/*
 * apic.c
 * everything apic related
 */

#include <kernel/x86/apic.h>

#include <kernel/x86/port.h>
#include <kernel/panic.h>
#include <kernel/klib.h>

#define PIC1_CMD 0x20
#define PIC2_CMD 0xa0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xa1

#define SPURIOUS_VECTOR 0xFF
#define APIC_ENABLE (0x1 << 8)

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
	outb(0xff, PIC1_DATA);
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

void eoi()
{
	uint32_t *eoireg = (uint32_t *)0xBFE000B0;
	*eoireg = 0x1;
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
	: "%eax", "%edx"
	);
	if (!(ret & (1 << 9)))
		panic("No APIC found.");

	/* Disable (mask) PIC*/
	mask_pic();

	/* 
	 * TODO: Map APIC address space (both local and io)
	 * to space below kernel (right before 0xc0000000)
	 */

	/* Get APIC ID for first CPU */
	uint32_t *apic_id = (uint32_t *)0xBFE00020;

	/* Keyboard Interrupt Vector (for INT 1)
	 * vector: 0x20  <-bits 0:7
	 * DELMOD: 0b000 <-bits 8:10
	 * DESTMOD: 0    <-bit 11
	 * Destnation Field: APIC ID of first CPU
	 */
	write_ioapic_reg(0x12, 0x21);
	write_ioapic_reg(0x13, (*apic_id << 24) & 0x0F000000);

	/* mask mouse input */
	write_ioapic_reg(0x28, 0x100FF);
	write_ioapic_reg(0x29, (*apic_id << 24) & 0x0F000000);

	/* Spurious Interrupt Vector */
	uint32_t *spurious_reg = (uint32_t *)0xBFF000F0;
	*spurious_reg = SPURIOUS_VECTOR + APIC_ENABLE;
	
	/* enable interrupts */
	__asm__("sti");
}
