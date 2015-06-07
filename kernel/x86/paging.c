/*
 *  paging.c
 *  Stuff about x86 paging.
 *  REMINDER: kmalloc is not x86 specific, paging is.
 */

#include <stdint.h>
#include <stdbool.h>

#include <kernel/x86/paging.h>

#include <kernel/panic.h>
#include <kernel/klib.h>

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

void probe_pages(){
	int mmap_length = mbstruct[11];
	mmap_addr = (struct mem_region *)(mbstruct[12] + 0xc0000000);

	if (mbstruct[0] && 0x40 != 0x40)
		panic("mmap is not present. Cannot make page list");

	uint32_t max_addr = mmap_addr[mmap_length / 0x18 - 1].base_addr -\
		    1 + mmap_addr[mmap_length / 0x18 - 1].length;
	uint32_t max_pages = max_addr / 0x1000 + 1;
	ps = (struct page_struct *)kmalloc(sizeof(struct page_struct) *\
		    max_pages);
	
	if (ps) {
		for (int i = 0; i < max_pages; i++) {
			ps[i].phys_addr = 0x1000 * i;
			ps[i].flags = 0x2; /* kernel, non-mapped */
		}
	} else
		panic("Failed to allocate memory for page structs");
	
	for (int i = 0; i < 0xc00; i++)
		ps[i].flags = 0x3; /* kernel, mapped */
}


/* TODO: Page allocating function. Check availability on map attempt. */
/* TODO: Ensure safety of multiboot data structure */
