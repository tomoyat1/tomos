/*
 *   pminit.c
 *   Setting things up in protected mode.
 *
 */


#include <stdint.h>

#include <kernel/pminit.h>

//flags: start at bit 8; bit 0 of input should be bit 8 of second lword
#define GDT_ENTRY(base, limit, flags)\
 (((base & (uint64_t )0xFF000000) << 32) |\
 ((flags & (uint64_t )0x0000F0FF) << 40) |\
 ((base & (uint64_t )0x00FFFFFF) << 16) |\
 ((limit & (uint64_t )0x000F0000) << 32) |\
 ((limit & (uint64_t )0x0000FFFF) << 0))

typedef struct
{
	uint16_t limit;	
	uint32_t base;
}__attribute__((packed)) gdt_ptr;

void set_gdt(void)
{
	static const uint64_t gdt[] __attribute__((aligned(16))) =
	{
		[0] 0x0,
		[1] = GDT_ENTRY(0x0, 0x000100000, 0xC09B),\
		[2] = GDT_ENTRY(0x0, 0x000100000, 0xC093),\
		[3] = GDT_ENTRY(0x0, 0x000100000, 0xC0FB),\
		[4] = GDT_ENTRY(0x0, 0x000100000, 0xC0F3)
	};
	static gdt_ptr sysgdt;
	sysgdt.base = (uint32_t)&gdt;
	sysgdt.limit = (uint16_t)5*8; //hardcoded. Need to change later.
	__asm__(
	"lgdtl %0"
	:
	:"m"(sysgdt)
	);
}


void pminit()
{
	//GDT
	set_gdt();
	//IDT
}
