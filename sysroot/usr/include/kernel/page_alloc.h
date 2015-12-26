#ifndef _PAGE_ALLOC_H_
#define _PAGE_ALLOC_H_

#include <stdint.h>
#include <stdbool.h>

#include <kernel/proc.h>

struct page_struct;

struct page_struct {
	uint32_t phys_addr;
	pid_t owner;
	struct page_struct *next;
	struct page_struct *prev;
};

/*
 * owner 
 * pid  0: kernel
 * pid  1: init
 * pid -1: non-mapped
 */

void probe_pages();

struct page_struct * alloc_page(int pow); 

#endif
