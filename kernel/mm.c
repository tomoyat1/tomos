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

#define C_ALLOCED_FLAG 1
#define IS_C_ALLOCED(chunk) (chunk->csize & C_ALLOCED_FLAG)
#define C_SIZE(chunk) (chunk->csize & -2)
#define IS_C_SUFFCIENT(chunk, alloc_size)                                            \
	(current->csize >= (alloc_size + C_HEADER_SIZE))
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
	size_t alloc_size = (bytes / 8 + 1) * 8;
	intptr_t newnext;
	intptr_t retaddr;
	intptr_t newbrk;
	intptr_t oldbrk;
	struct chunk *fc = kernel_thread->start_heap;
	if (sbrk(0) == kernel_thread->start_heap) {
		sbrk(0x1000);
		fc->csize = 0x1000 - C_HEADER_SIZE;
		fc->next = NULL;
		fc->prev = NULL;
	}
	struct chunk *current = (struct chunk *)(kernel_thread->start_heap);
FIND:
	while (1) {
		if (IS_C_SUFFCIENT(current, alloc_size)
				&& !IS_C_ALLOCED(current)) {
			printk("found");
			goto FOUND;
		}
		if (!current->next)
			break;
		current = current->next;
	}
	oldbrk = (intptr_t)sbrk(0);
	if ((newbrk = (intptr_t)sbrk(SBRK_INC(alloc_size)))) {
		if (IS_C_ALLOCED(current)) {
			current->next = (struct chunk *)oldbrk;
			current->next->prev = current;
			current = current->next;
			current->next = NULL;
			current->csize = newbrk - oldbrk - C_HEADER_SIZE;
		} else {
			current->csize += (newbrk - oldbrk);
		}
		goto FIND;
	}
	return NULL;

FOUND:
	newnext= (intptr_t)current + C_HEADER_SIZE + current->csize;
	current->next = (struct chunk *)newnext;
	current->next->csize = current->csize - alloc_size - C_HEADER_SIZE;
	current->next->prev = current;
	current->csize = alloc_size | C_ALLOCED_FLAG;

	retaddr = (intptr_t)current + C_HEADER_SIZE;
	return (void *)retaddr;
}


void kfree(void *ptr)
{
	struct chunk *target = (struct chunk *)((intptr_t)ptr - C_HEADER_SIZE);
	target->csize = C_SIZE(target);

	/* Room exists for refactoring */
	if (target->next && !IS_C_ALLOCED(target->next)) {
		target->csize += (target->next->csize + C_HEADER_SIZE);
		target->next = target->next->next;
	}
	if (!IS_C_ALLOCED(target->prev)) {
		target = target->prev;
		target->csize += (target->next->csize + C_HEADER_SIZE);
		target->next = target->next->next;
	}
}
