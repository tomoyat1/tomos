/*
 * mm.c
 * memory management. get free memory, release memory, all the good stuff. 
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kernel/panic.h>
#include <kernel/page_alloc.h>
#include <kernel/proc.h>
#include <kernel/klib.h>
#include <kernel/kstring.h>

#include <kernel/brk.h>

#define C_ALLOCED 1
#define C_SIZE(chunk) (chunk->csize & -2)
#define C_HEADER_SIZE sizeof(struct chunk)
#define SBRK_INC(x)                                                            \
	((((x - C_SIZE(current) + C_HEADER_SIZE)/ 0x1000) + 1) * 0x1000)
#define OLDBRK                                                                 \
	((intptr_t)current + C_HEADER_SIZE + C_SIZE(current))

extern void *_end;
extern uint32_t *mbstruct;
extern struct proc_struct *kernel_thread;

/* Heap should be 0x1000 (one page) aligned.*/

struct chunk {
	uint32_t csize;
	struct chunk *next;
	struct chunk *prev;
};

void mminit()
{
	/* Set top of heap in kernel proc struct  */
	kernel_thread->start_heap = (void *)&_end;
	kernel_thread->brk = (void *)&_end;

	/* detect memory size and create page structs */
	probe_pages();
}

void *kmalloc(size_t bytes)
{
	/*
	 * Force 4 byte alignment for kmalloc
	 * Implement as first-fit for now, make best fit later
	 *
	 * Heap initialization in kmalloc function for now.
	 * Gotta find a better place for this.
	 *
	 * Memory given by page allocator SHOULD BE ZEROED
	*/
	size_t alloc_size = (bytes / 8 * 8) + 1;
	intptr_t newnext;
	intptr_t retaddr;
	intptr_t newbrk;
	struct chunk *fc = kernel_thread->start_heap;
	if (sbrk(0) == kernel_thread->start_heap) {
		(struct chunk *)sbrk(0x1000);
		fc->csize = 0x1000 - C_HEADER_SIZE;
		fc->next = NULL;
		fc->prev = NULL;
	}
	struct chunk *current = (struct chunk *)(kernel_thread->start_heap);
FIND:
	while (1) {
		if (current->csize >= (alloc_size + C_HEADER_SIZE)) {
			printk("found");
			goto FOUND;
		}
		if (!current->next)
			break;
		current = current->next;
	}
	if ((newbrk = (intptr_t)sbrk(SBRK_INC(alloc_size)))) {
		if (current->csize & C_ALLOCED) {
			newnext = OLDBRK;
			current->next = (struct chunk *)OLDBRK;
			current->next->prev = current;
			current = current->next;
			current->next = NULL;
			current->csize = newbrk - OLDBRK - C_HEADER_SIZE;
		} else {
			current->csize += (newbrk - OLDBRK);
		}
		goto FIND;
	}
	return NULL;

FOUND:
	current->csize = alloc_size | C_ALLOCED;
	newnext= (intptr_t)current + C_HEADER_SIZE + current->csize;
	current->next = (struct chunk *)newnext;
	current->next->prev = current;

	retaddr = (intptr_t)current + C_HEADER_SIZE;
	return (void *)retaddr;
}
