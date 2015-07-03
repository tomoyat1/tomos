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
#include <kernel/x86/paging.h>
#include <kernel/klib.h>

extern void *kernel_heap;
extern uint32_t *mbstruct;
extern struct proc_struct *kernel_thread;

/* Heap should be 0x1000 (one page) aligned.*/

void mminit()
{
	/* initialize kernel heap */
	init_heap(&kernel_heap);
	/* detect memory size and create page structs */
	probe_pages();
	/* Set top of heap in kernel proc struct  */
	kernel_thread->proc_heap = (struct bh *)&kernel_heap;
}

void init_heap(void *heap_base)
{
	struct bh *first_block = (struct bh *)heap_base;
	first_block->size = 0xc0c00000 - (size_t)heap_base - sizeof(struct bh);
	first_block->next = NULL;
}

void add_heap(void *heap_base)
{
	
}

void *alloc_free(size_t demand_size, void *heap_base)
{
	struct bh *current = (struct bh *)heap_base;
	struct bh *old_next = current->next;

	size_t alloc_size = demand_size + (demand_size % 2);
	size_t alloc_size_w_bh = alloc_size + HEADER_SIZE;

	do {
		if (current->size == alloc_size)
			goto FOUND_EXACT;
		else if (current->size >= alloc_size_w_bh\
					&& current->size % 2 == 0)
			goto FOUND_GREATER;
		else if (current->next == NULL)
			goto FAIL;
		else 
			current = current->next;

	} while(true);

FAIL:
	return NULL;

FOUND_EXACT:
	current->size = current->size - 1;

FOUND_GREATER:
	current->next = (struct bh *)((char *)current + alloc_size_w_bh);
	current->next->size = current->size - alloc_size_w_bh;
	current->size = alloc_size - 1;
	if (!old_next)
		current->next->next = NULL;
	else
		current->next->next = old_next;

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
