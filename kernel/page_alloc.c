/*
 * page_alloc.c
 * Architecture independent page allocator.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <kernel/page_alloc.h> 
#include <kernel/panic.h>
#include <kernel/klib.h>

extern uint32_t kernelpagedir;
extern uint32_t *mbstruct;

struct mem_region {
	uint32_t size;
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
};

static struct mem_region *mmap_addr;
static struct page_struct *ps;

static uint32_t mmap_length;

void probe_pages()
{
	mmap_length = mbstruct[11];
	mmap_addr = (struct mem_region *)(mbstruct[12] + 0xc0000000);

	if (mbstruct[0] && 0x40 != 0x40)
		panic("mmap is not present. Cannot make page list");

	uint32_t max_addr = mmap_addr[mmap_length / 0x18 - 1].base_addr -\
		    1 + mmap_addr[mmap_length / 0x18 - 1].length;
	uint32_t max_pages = max_addr / 0x1000 + 1;
	ps = (struct page_struct *)kmalloc(sizeof(struct page_struct) *\
		    max_pages);
	
	//printaddr((int)ps);
	printk("\n");
	if (ps) {
		for (int i = 0; i < max_pages; i++) {
			ps[i].phys_addr = 0x1000 * i;
			ps[i].flags = 0x2; /* kernel, non-mapped */
			ps[i].next = (struct page_struct *)(&ps[i + 1]);
		}
	} else {
		panic("Failed to allocate memory for page structs");
	}

	ps[max_pages - 1].next = NULL;

	/* Assuming first three 4MiB pages are allocated at boot. */
	for (int i = 0; i < 0xc00; i++)
		ps[i].flags = 0x3; /* kernel, mapped */

	/* Assume ISA memory hole */
	for (int i = 0xf00; i < 0x1000; i++)
		ps[i].flags = 0x6; /* kernel, non-mapped, io-mapped */
}

struct page_struct * page_alloc(size_t contiguous)
{
	int fp = 0;
	bool fp_set = false;
	bool sufficient;

	/* first fit allocation */
	for (int i = 0; ps[i].next != NULL; i++) {
		if ((ps[i].flags & 0x5) == 0 && !fp_set) {
			fp = i;
			fp_set = true;
		}
		if (i - fp >= contiguous) {
			sufficient = true;
			break;
		}
	}

	if (sufficient)
		goto FOUND;

	return NULL;
FOUND:
	for (int i = 0; i < contiguous; i++) {
		ps[fp + i].flags = ps[fp + i].flags | 0x1;
	}
	return (ps + fp);
}


void page_free(struct page_struct *page)
{
	page->flags = page->flags & 0x6;
}

/* TODO: Ensure safety of multiboot data structure */
