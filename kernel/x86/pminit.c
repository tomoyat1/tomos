/*
 *   pminit.c
 *   Setting things up in protected mode.
 *
 */


#include <stdint.h>

#include <kernel/x86/pminit.h>

#include <kernel/x86/asm/segsel.h>
#include <kernel/x86/interrupts.h>

//flags: start at bit 8; bit 0 of input should be bit 8 of second lword
#define GDT_ENTRY(base, limit, flags)\
 (((base & (uint64_t )0xFF000000) << 32) |\
 ((flags & (uint64_t )0x0000F0FF) << 40) |\
 ((base & (uint64_t )0x00FFFFFF) << 16) |\
 ((limit & (uint64_t )0x000F0000) << 32) |\
 ((limit & (uint64_t )0x0000FFFF) << 0))


struct gdt_ptr
{
	uint16_t limit;	
	uint32_t base;
}__attribute__((packed));

extern struct tss_struct tss;

void set_gdt(void)
{
	static uint64_t gdt[] __attribute__((aligned(16))) =
	{
		[0] = 0x0,\
		[1] = GDT_ENTRY(0x0, 0x0000FFFFF, 0xC09D),\
		[2] = GDT_ENTRY(0x0, 0x0000FFFFF, 0xC093),\
		[3] = GDT_ENTRY(0x0, 0x0000FFFFF, 0xC0FB),\
		[4] = GDT_ENTRY(0x0, 0x0000FFFFF, 0xC0F3),\
		[5] = 0x0 /* Initialize with 0x0 for now.*/
	};
	gdt[5] = GDT_ENTRY((uint32_t)&tss, sizeof(struct tss_struct), 0x89);
	static struct gdt_ptr sysgdt;
	sysgdt.base = (uint32_t)&gdt;
	sysgdt.limit = (uint16_t)sizeof(gdt);
	__asm__(
	"lgdtl %0;"
	:
	:"m"(sysgdt)
	);

	segsel();
}
//init segment selectors with RPL 0 for now. Implement protection later.

void pminit()
{
	set_gdt(); /* GDT */
	set_tss(); /* TSS */
	set_idt(); /* IDT */
}
