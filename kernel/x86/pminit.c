/*
 *   pminit.c
 *   Setting things up in protected mode.
 *
 */


#include <stdint.h>

#include <kernel/x86/pminit.h>

#include <kernel/x86/asm/segsel.h>
#include <kernel/panic.h>

//flags: start at bit 8; bit 0 of input should be bit 8 of second lword
#define GDT_ENTRY(base, limit, flags)\
 (((base & (uint64_t )0xFF000000) << 32) |\
 ((flags & (uint64_t )0x0000F0FF) << 40) |\
 ((base & (uint64_t )0x00FFFFFF) << 16) |\
 ((limit & (uint64_t )0x000F0000) << 32) |\
 ((limit & (uint64_t )0x0000FFFF) << 0))

extern void *kernel_stack_top;

struct tss_struct
{
	uint32_t prev_task_link;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt_selector;
	uint32_t io_map_base;
} __attribute__((packed));

struct tss_struct tss;

struct gdt_ptr
{
	uint16_t limit;	
	uint32_t base;
}__attribute__((packed));

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

void set_tss()
{
	tss.ss0 = 0x10;
	tss.esp0 = (uint32_t)&kernel_stack_top;
	
	__asm__(
	"movw $0x28, %%cx;"
	"ltr %%cx;"
	:
	:
	:"cx"
	);
}


void pminit()
{
	//GDT
	set_gdt(); /* GDT */
	set_tss(); /* TSS */
	//set_idt(); /* IDT */
}
