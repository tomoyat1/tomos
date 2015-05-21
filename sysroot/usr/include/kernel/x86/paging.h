#ifndef _PAGING_H_
#define _PAGING_H_

#include <stdint.h>
#include <stdbool.h>

struct page_struct
{
	uint32_t phys_addr;
	uint32_t flags;	
};

/*
 * flags:
 * bit 1: mapped
 * bit 2: permissions, 1 for kernel, 0 for user.
 */

void probe_pages(int *mbheader);

#endif

