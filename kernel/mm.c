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
	char *test;
	/* Set top of heap in kernel proc struct  */
	kernel_thread->start_heap = (void *)&_end;
	kernel_thread->brk = (void *)&_end;

	/* malloc test */
	test = (char *)kmalloc(16);
	kstrcpy(test, "foobar");
	__asm__("cli;hlt");
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
	if (sbrk(0) == kernel_thread->brk) {
		struct chunk *fc = (struct chunk *)sbrk(0x1000);
		fc->csize = 0x1000 - sizeof(struct chunk);
		fc->next = NULL;
		fc->prev = NULL;
	}
	struct chunk *current = (struct chunk *)(kernel_thread->start_heap);
	while (current) {
		if (current->csize >= (alloc_size + sizeof(struct chunk))) {
			printk("found");
			goto FOUND;
		}
		current = current->next;
	}
	return NULL;

FOUND:
	current->csize = alloc_size | C_ALLOCED;
	newnext= (intptr_t)current + current->csize + sizeof(struct chunk);
	current->next = (struct chunk *)newnext;
	current->next->prev = current;

	retaddr = (intptr_t)current + sizeof(struct chunk);
	return (void *)retaddr;
}
