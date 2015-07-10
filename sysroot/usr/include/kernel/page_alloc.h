#ifndef _PAGE_ALLOC_H_
#define _PAGE_ALLOC_H_

#include <stdint.h>
#include <stdbool.h>

struct page_struct;

struct page_struct {
	uint32_t phys_addr;
	uint32_t flags;	
	struct page_struct *next;
};

/*
 * flags:
 * bit 1: paged in
 * bit 2: permissions, 1 for kernel, 0 for user.
 * bit 3: io mapped
 */

void probe_pages();

struct page_struct * page_rq(); 

#endif
