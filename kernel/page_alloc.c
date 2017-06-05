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

#ifdef x86
#include <kernel/x86/vm.h>
#endif

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
			if (ps[i].phys_addr != 0x1000 * i)
				panic("memory error");
			ps[i].owner = -1; /* Free to take */
			ps[i].next = (struct page_struct *)&ps[i+1];
			ps[i].prev = (struct page_struct *)&ps[i-1];
		}
	} else
		panic("Failed to allocate memory for page structs");

	ps[max_pages - 1].next = NULL;
	ps[0].prev = NULL;

	/* Assuming first three 4MiB pages are allocated at boot. */
	for (int i = 0; i < 0xc00; i++)
		ps[i].owner = 0; /* kernel */

	/* Detect IO mapped memory regions and flag them as so */
	printk("success!");
}

/* Find free page_frame and do architecture specific mapping */
struct page_struct * alloc_page(int pow)
{
	int size = 1;
	int cont = 0;
	struct page_struct *f;
	for (int i = 0; i < pow; i++) {
		size *= 2;
	}
	for (int i = 0;;i++) {
		if (ps[i].next == NULL)
			goto FAIL;
		if (ps[i].owner == -1) {
			if (!f)
				f = &ps[i];
			cont++;
			/* Assume kernel for now  */
			ps[i].owner = 0;
			if (cont == size)
				break;
		} else {
			f = NULL;
		}
	}

	return f;

FAIL:
	panic("Out of physical memory.");
	return NULL;
}
