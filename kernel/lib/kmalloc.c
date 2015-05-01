/*
 *
 *  kmalloc.c
 *  TODO: use architecture specific memory code to implement kmalloc 
 *  keep track of allocated memory in kernel data structure (linked list)
 *  
 */

#include <stddef.h>

#include <kernel/kmalloc.h>

#include <kernel/mm.h>

void* kmalloc(size_t bytes)
{
	free_mem_t* block = search_free_mem(BYTES_TO_CHUNKS(bytes));	
	void* mem_addr = get_free_mem_from_block(block, BYTES_TO_CHUNKS(bytes)); 
	return mem_addr;
}
