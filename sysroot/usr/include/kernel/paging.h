#ifndef _PAGING_H_
#define _PAGING_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct page_struct page_struct_t;

typedef struct page_struct
{
	uint32_t index;
	bool is_mapped;	
	uint32_t allocated_bytes[128];
	page_struct_t *next;
	page_struct_t *prev;
} page_struct_t;

void page_alloc();
#endif

