/*
 * mm.c
 * memory management. get free memory, release memory, all the good stuff. 
 */
#include <kernel/proc.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kernel/panic.h>
#include <kernel/page_alloc.h>
#include <kernel/klib.h>

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
	/* detect memory size and create page structs */
	probe_pages();
	/* Set top of heap in kernel proc struct  */
	kernel_thread->proc_heap = (struct bh *)&_end;
}

void *kmalloc(size_t bytes)
{
	return (void *)0x0;
}
