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
free_mem_t first_free_mem_chunk;

void mminit()
{
	first_free_mem_chunk.next = NULL;
	first_free_mem_chunk.prev = NULL;	
	first_free_mem_chunk.start_addr = (char *)&heap;
	first_free_mem_chunk.size = 0xC0800000 - (size_t)&heap; //To end of first 2 4MiB pages.
}

free_mem_t* search_free_mem(size_t bytes)
{
	bool found = false;
	free_mem_t *current = &first_free_mem_chunk;
	do
	{
		if( current->size >= bytes ){
			found = true;
			return current;	
		}else{
			if( !current->next)
				goto NOMEM;
			*current = *current->next;
		}
	}while(found == false);
NOMEM:
	panic("NO ALLOCATEABLE MEMORY");
	return NULL;
}
