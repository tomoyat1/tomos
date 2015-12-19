/*
 * page_alloc.c
 * Architecture independent page allocator.
 */
#include <stdint.h>
#include <stdbool.h>

#include <kernel/page_alloc.h> 
#include <kernel/panic.h>
#include <kernel/mm.h>
#include <kernel/klib.h>

extern uint32_t kernelpagedir;
extern uint32_t *mbstruct;

struct mem_region
{
	uint32_t size;
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
};

static struct mem_region *mmap_addr;
static struct page_struct *ps;

static uint32_t mmap_length;

void probe_pages(){
	int i;
	mmap_length = mbstruct[11];
	mmap_addr = (struct mem_region *)(mbstruct[12] + 0xc0000000);

	if (mbstruct[0] && 0x40 != 0x40)
		panic("mmap is not present. Cannot make page list");

	uint32_t max_addr = mmap_addr[mmap_length / 0x18 - 1].base_addr -\
		    1 + mmap_addr[mmap_length / 0x18 - 1].length;
	uint32_t max_pages = max_addr / 0x1000 + 1;
	ps = (struct page_struct *)kmalloc(sizeof(struct page_struct) *\
		    max_pages);
	
	if (ps) {
		for (i = 0; i < max_pages; i++) {
			ps[i].phys_addr = 0x1000 * i;
			/*
			 * When kernel page faults on access to
			 * 0x00108000, the following test fails
			 * success -> memory limit off by 2 bits
			 * perhaps qemu bugginess
			 * failure -> ps is 0xbfe000b0
			 * this is the same as local apic eoi reg
			 */
			if (ps[i].phys_addr != 0x1000 * i)
				panic("memory error");
			ps[i].flags = 0x2; /* kernel, non-mapped */
			ps[i].next = (struct page_struct *)(0x1000 * (i + 1));
		}
	} else
		panic("Failed to allocate memory for page structs");

	ps[max_pages - 1].next = NULL;

	/* Assuming first three 4MiB pages are allocated at boot. */
	for (int i = 0; i < 0xc00; i++)
		ps[i].flags = 0x3; /* kernel, mapped */

	/* Detect IO mapped memory regions and flag them as so */
	printk("success!");
	__asm__("cli;hlt");
	
}
