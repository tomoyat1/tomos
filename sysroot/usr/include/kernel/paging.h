#ifndef _PAGING_H_
#define _PAGING_H_

#include <stdint.h>
#include <stdbool.h>

struct page_struct;

struct page_struct
{
	uint32_t index;
	bool is_mapped;	
	uint32_t allocated_bytes[128];
	struct page_struct *next;
	struct page_struct *prev;
};

void page_alloc();
#endif

