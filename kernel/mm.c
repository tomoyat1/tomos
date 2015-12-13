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

extern void *kernel_heap;
extern uint32_t *mbstruct;
extern struct proc_struct *kernel_thread;

/* Heap should be 0x1000 (one page) aligned.*/

void mminit()
{
	/* detect memory size and create page structs */
	probe_pages();
	/* Set top of heap in kernel proc struct  */
	kernel_thread->proc_heap = (struct bh *)&kernel_heap;
}
