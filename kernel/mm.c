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
#include <stdbool.h>

#include <kernel/panic.h>

extern char* heap;
//This thing is in .data section. The ones following will be kmalloc'd
free_mem_t first_free_mem_block;

void mminit()
{
	first_free_mem_block.next = NULL;
	first_free_mem_block.prev = NULL;	
	first_free_mem_block.start_addr = (char *)&heap;
	first_free_mem_block.size = 0xC0800000 - (size_t)&heap; //To end of first 2 4MiB pages.
}
//Need to expand function to deal with userspace heaps too.
free_mem_t* search_free_mem(size_t chunks)
{
	bool found = false;
	free_mem_t *current = &first_free_mem_block;
	do
	{
		if( current->size >= chunks * CHUNK_SIZE ){
			found = true;
		}else{
			if( !current->next)
				goto NOMEM;
			*current = *current->next;
		}
	}while(found == false);
	return current;
NOMEM:
	panic("NO ALLOCATEABLE MEMORY");
	return NULL;
}

void* get_free_mem_from_block(free_mem_t* mem, size_t chunks)
{
	//potentiallly 16 bytes to represent 1 byte of free memory??
	//In your dreams!!	
	//Solution: Allocate and free memory in 16byte chunks. 
	mem->start_addr = (char *)((size_t)mem->start_addr + (chunks * CHUNK_SIZE));
	return (void *)mem->start_addr;
}
