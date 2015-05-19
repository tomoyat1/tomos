/*
 *  kfree.c
 *  freeing memory. 
 */

#include <stddef.h>

#include <kernel/lib/kfree.h>

#include <kernel/mm.h>
 
extern void *kernel_heap;

void kfree(void *addr)
{
	free_allocated(addr, &kernel_heap);
}
