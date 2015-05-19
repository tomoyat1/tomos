/*
 * mm.c
 * memory management. get free memory, release memory, all the good stuff. 
 */

#include <kernel/mm.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kernel/panic.h>

#include <kernel/klib.h>

/* Heap should be 0x1000 (one page) aligned.*/

void mminit(void *heap_base)
{
	bh_t *first_block = (bh_t *)heap_base;
	first_block->size = 0xc0800000 - (size_t)heap_base - sizeof(bh_t);
	first_block->next = NULL;
}

void *alloc_free(size_t demand_size, void *heap_base)
{
	bh_t *current = (bh_t *)heap_base;
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
		return (char *)current + HEADER_SIZE;
	} else {
		current->next = (bh_t *)((char *)current + \
		    HEADER_SIZE + alloc_size);
		current->next->size = current->size - alloc_size - \
		    HEADER_SIZE;
		current->size = alloc_size ^ 0x1;
		current->next->next = NULL;
		return (char *)current + HEADER_SIZE;
	}

	/* Mysteriously, execution ended up here... */
	goto FAIL;
}

void free_allocated(void *addr, void *heap_base)
{
	/*
	 * Match addr with start_addr in each allocated block, get position
	 * of size, and set free bit to 0.
	 */
	size_t target_addr = (size_t)addr - sizeof(bh_t);
	bh_t *target = (bh_t *)target_addr;
	target->size = target->size ^ 0x1;

	/* 
	 * Block merging logic. Absorb next into block just freed.
	 * Might need prev pointer for each bh_t
	 */
	if (target->next->size % 2 == 0) {
		target->size = target->size + sizeof(bh_t) + target->next->size;
		if( target->next->next == NULL) {
			target->next = NULL;
		} else {
			target->next = target->next->next;
		}
	}
}
