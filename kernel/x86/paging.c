/*
 *  paging.c
 *  Stuff about x86 paging.
 *  REMINDER: kmalloc is not x86 specific, paging is.
 */

#include <stdint.h>
#include <stdbool.h>

#include <kernel/paging.h>

#include <kernel/panic.h>
#include <kernel/klib.h>

/*
 * Get memory map from multiboot loader
 * Make page struct for each 4KiB page.
 */

struct mem_region
{
	uint32_t size;
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
};

void probe_pages(int *mbheader){
	int mmap_length = mbheader[11];
	/*Need to adjust mmap address to be higher half.*/
	struct mem_region *mmap_addr =\
		    (struct mem_region *)(mbheader[12] + 0xc0000000);

	if (mbheader[0] && 0x40 != 0x40)
		panic("mmap is not present. Cannot make page list");

	uint32_t max_addr = mmap_addr[mmap_length / 0x18 - 1].base_addr - 1 +\
		    mmap_addr[mmap_length / 0x18 - 1].length;
	uint32_t max_pages = max_addr / 0x1000 + 1;
	struct page_struct *ps =\
		    (struct page_struct *)kmalloc(sizeof(struct page_struct) *\
		    max_pages);
	
	if (ps) {
		for (int i = 0; i < max_pages; i++) {
			ps[i].phys_addr = 0x1000 * i;
			ps[i].flags = 0x2; /* kernel, non-mapped */
		}
	} else
		panic("Failed to allocate memory for page structs");

}
