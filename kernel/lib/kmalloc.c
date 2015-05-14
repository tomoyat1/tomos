/*
 *
 *  kmalloc.c
 *  TODO: use architecture specific memory code to implement kmalloc 
 *  keep track of allocated memory in kernel data structure (linked list)
 *  
 */

#include <stddef.h>

#include <kernel/lib/kmalloc.h>

#include <kernel/mm.h>

extern void *kernel_heap;

void* kmalloc(size_t bytes)
{
	void* block_addr = alloc_free(bytes, &kernel_heap);	
	return block_addr;
}

