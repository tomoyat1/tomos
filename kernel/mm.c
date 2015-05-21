/*
 * mm.c
 * memory management. get free memory, release memory, all the good stuff. 
 */

#include <kernel/mm.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kernel/panic.h>
#include <kernel/x86/paging.h>
#include <kernel/klib.h>

extern void *kernel_heap;

/* Heap should be 0x1000 (one page) aligned.*/

void mminit(int *mbheader)
{
	/* initialize kernel heap */
	init_heap(&kernel_heap);
	/* detect memory size and create page structs */
	probe_pages(mbheader);
}

void init_heap(void *heap_base)
{
	struct bh *first_block = (struct bh *)heap_base;
	first_block->size = 0xc1000000 - (size_t)heap_base - sizeof(struct bh);
	first_block->next = NULL;
}

void *alloc_free(size_t demand_size, void *heap_base)
{
	struct bh *current = (struct bh *)heap_base;
	size_t alloc_size = demand_size + (demand_size % 2);

	do {
		if ((current->size >= alloc_size) &&( current->size % 2 == 0))
			goto FOUND;
		else {
			if (current->next == NULL)
				goto FAIL;
			else
				current = current->next;
		}
	} while(true);

FAIL:
	return NULL;

FOUND:
	if (alloc_size == current->size) {
		current->size = current->size ^ 0x1;
	} else {
		current->next = (struct bh *)((char *)current + \
		    HEADER_SIZE + alloc_size);
		current->next->size = current->size - alloc_size - \
		    HEADER_SIZE;
		current->size = alloc_size ^ 0x1;
		current->next->next = NULL;
	}

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
	size_t target_addr = (size_t)addr - sizeof(struct bh);
	struct bh *target = (struct bh *)target_addr;
	target->size = target->size ^ 0x1;

	/* 
	 * Block merging logic. Absorb next into block just freed.
	 * Might need prev pointer for each struct bh
	 */
	if (target->next->size % 2 == 0) {
		target->size = target->size + sizeof(struct bh) + target->next->size;
		if( target->next->next == NULL) {
			target->next = NULL;
		} else {
			target->next = target->next->next;
		}
	}
}
