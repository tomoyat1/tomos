/*
 *
 * memory management. get free memory, release memory, all the good stuff. 
 *
 *
 *
 */


#include <kernel/mm.h>

#include <stddef.h>
#include <stdint.h>

extern char* heap;

free_mem_t first_free_mem_chunk;

void mminit()
{
	first_free_mem_chunk.next = NULL;
	first_free_mem_chunk.prev = NULL;	
	first_free_mem_chunk.start_addr = (char *)&heap;
	first_free_mem_chunk.size = 0xC0800000 - (size_t)&heap; //To end of first 2 8MiB pages.
}

free_mem_t* search_free_mem(size_t bytes)
{
	return (free_mem_t *)1;	
}
