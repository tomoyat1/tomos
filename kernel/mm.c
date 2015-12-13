/*
 * mm.c
 * memory management. get free memory, release memory, all the good stuff. 
 */

#include <kernel/mm.h>
#include <kernel/proc.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kernel/panic.h>
#include <kernel/page_alloc.h>
#include <kernel/klib.h>

//extern void *kernel_heap;
extern char *kernelpagedir;
extern uint32_t *mbstruct;
extern struct proc_struct *kernel_thread;

/* Heap should be 0x1000 (one page) aligned.*/

void mminit(void *heap_base)
{
	/* initialize kernel heap */
	init_heap(heap_base);
	/* detect memory size and create page structs */
	probe_pages();
	/* Set top of heap in kernel proc struct  */
	kernel_thread->proc_heap = (struct bh *)heap_base;
}

void init_heap(void *heap_base)
{
	struct bh *first_block = (struct bh *)heap_base;
	first_block->size = 0xc0c00000 - (size_t)heap_base - sizeof(struct bh);
	printk("heap_base: ");
	printaddr((int)heap_base);
	printk("\n");
	first_block->next = NULL;
}

int expand_heap(void *heap_base)
{
	/* TODO: Separate x86-specific logic to x86/paging.c */
	/* TODO: Consider errors in physical page allocation. */

	/* Get current heap limit */
	struct bh *last = (struct bh *)heap_base;
	while (last->next != NULL) {
		last = last->next;
	}
	size_t heap_limit = (size_t)last + last->size + HEADER_SIZE;

	uint32_t pde_index = heap_limit / 0x400000;
	uint32_t pte_index = (heap_limit % 0x400000) / 0x1000;
	uint32_t *pd_virt = (uint32_t *)(&kernelpagedir + 0xc0000000);
	uint32_t *pt_virt;
	uint32_t pt_phys;
	if ((pd_virt[pde_index] & 0x1) == 0) {
		struct page_struct *pte = page_alloc(1);
		pd_virt[pde_index] = pte->phys_addr;
		pd_virt[pde_index] = pd_virt[pde_index] | 0x3;
		pt_phys = pte->phys_addr; /* This ain't virtual address, it is phsycal address. */
	} else {
		pt_phys = pd_virt[pde_index] & 0xFFFFF000; /* Same as above. */
	}

	/*
	   Chicken or egg. Need to page in pt in order to page in new page of
	   heap.
	   Need to quickly map pt, write to it, and then unmap it.
	*/
	pd_virt[1023] = pt_phys / 0x00400000;
	pd_virt[1023] = pd_virt[1023] | 0x83;
	pt_virt = (uint32_t *)((pt_phys % 0x00400000) | 0xFFC00000);

	struct page_struct *new_page = page_alloc(1);
	pt_virt[pte_index] = new_page->phys_addr; /* page fault on this line. */
	pt_virt[pte_index] = pt_virt[pte_index] | 0x3;
	pd_virt[1023] = 0x0;
	printk("Should have new page\n");

	if (last->size % 2) {
		panic("not done");
	} else {
		last->size += 0x1000;
	}

	return 1;
}

void *alloc_free(size_t demand_size, void *heap_base)
{
	struct bh *current = (struct bh *)heap_base;
	struct bh *old_next = current->next;

	size_t alloc_size = demand_size + (demand_size % 2);
	size_t alloc_size_w_bh = alloc_size + HEADER_SIZE;

SEARCH:
	do {
		if (current->size == alloc_size) {
			goto FOUND_EXACT;
		} else if (current->size >= alloc_size_w_bh\
					&& current->size % 2 == 0) {
			goto FOUND_GREATER;
		} else if (current->next == NULL) {
			goto FAIL;
		} else {
			current = current->next;
			old_next = old_next->next;
		}

	} while(true);

FAIL:
	if (expand_heap(heap_base)) {
		goto SEARCH;
	} else {
		/* If efforts above are in vain... */
		return NULL;
	}

FOUND_EXACT:
	current->size = current->size - 1;

FOUND_GREATER:
	current->next = (struct bh *)((char *)current + alloc_size_w_bh);
	current->next->size = current->size - alloc_size_w_bh;
	current->size = alloc_size - 1;
	if (!old_next)
		current->next->next = NULL;
	else
		current->next->next = current->next;

	/* returns 0x0 ...wtf? */
	return (char *)current + HEADER_SIZE;

	/* Mysteriously, execution ended up here... */
	goto FAIL;
}

void free_allocated(void *addr, void *heap_base)
{
	/*
	 * Match addr with start_addr in each allocated block, get position
	 * of size, and set free bit to 0.
	 */
	size_t target_addr = (size_t)addr - HEADER_SIZE;
	struct bh *target = (struct bh *)target_addr;
	target->size = target->size + 1;

	/* 
	 * Block merging logic. Absorb next into block just freed.
	 * Might need prev pointer for each struct bh
	 */
	if (target->next->size % 2 == 0) {
		target->size = target->size + HEADER_SIZE + target->next->size;
		if( target->next->next == NULL) {
			target->next = NULL;
		} else {
			target->next = target->next->next;
		}
	}
}
