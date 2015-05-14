/*
 * mm.c
 * memory management. get free memory, release memory, all the good stuff. 
 */


#include <kernel/mm.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kernel/panic.h>

#include <kernel/klib.h>

extern void *kernel_heap;
//heap should be 0x1000 (one page) aligned.

void mminit()
{
	size_t init_num_of_pages = (0xC0800000 - (size_t)&kernel_heap) / 0x1000; //To end of first 2 4MiB pages.
	for ( uint32_t pages = 0; pages < init_num_of_pages - 1; pages++ )
	{
		bitmap_for_page_t *pagemap = (bitmap_for_page_t *)((uint32_t)&kernel_heap + (uint32_t)(0x1000 * pages));
		pagemap->next_page = (bitmap_for_page_t *)((uint32_t)pagemap + (uint32_t)0x1000);

		//zero out each bitmap except for the first 516 bits, which is the size of bitmap_for_page_t
		for ( uint32_t i = 0; i < 128; i++ )
		{
			pagemap->bitmap[i] = 0;
		}

		uint32_t bits = sizeof(bitmap_for_page_t);
		uint32_t longword = bits / 32;
		uint32_t offset = bits % 32;

		//first the 32bit chunks
		for ( uint32_t i = 0; i < longword; i++ )
		{
			pagemap->bitmap[i] = 0xFFFFFFFF;
		}

		//next the leftovers
		for ( uint32_t i = 0; i < offset; i++ )
		{
			uint32_t mask = 1 << i;
			pagemap->bitmap[longword] = pagemap->bitmap[longword] | mask;
		}
	}
	//TODO: make last bitmap_for_page_t's next pointer NULL
}

void* alloc_free(size_t size, void *heap_base)
{
	//bool found = false;
	bitmap_for_page_t *current_page = (bitmap_for_page_t *)heap_base;
	uint32_t longword; //longword in array.
	uint32_t mask;
	uint32_t shifts;  //offset within longword.

	uint32_t return_addr;

	do
	{
		for ( longword = 0; longword < 128; longword++ ) 
		{
			//if not all 1s, search for adequate free region.
			if ( current_page->bitmap[longword] != 0xFFFFFFFF )
			{
				mask = ~(pow(2,size) - 1);
				for ( shifts = 0; shifts < 31 - size; shifts++ )
				{
					//bitwise or bitmap with mask, then xor that with mask.
					if ( ((current_page->bitmap[longword] | mask) ^ mask) == 0 )
					{
						goto DONE;
					}
					mask = mask <<1;
				};
			}
		}
		current_page++;
	}while( current_page->next_page == NULL );
	goto ERROR;

DONE:
	return_addr = (uint32_t)current_page + 32*longword + shifts;
	return (void *)return_addr;

ERROR:
	panic("OUT OF MEMORY");
	return NULL;
}

void free_allocated(void *addr, void *heap_base)
{
	//shit, sizeof ain't work. need to keep track of alloc'd region's sizes. shit shit shit.
}
