#ifndef _PROC_H_
#define _PROC_H_

#include <kernel/mm.h>
#include <kernel/page_alloc.h>
#include <kernel/types.h>

/* Proc struct. should be placed directly past top of stack. */

struct proc_struct;
struct proc_struct {
	struct proc_struct *next;
	struct proc_struct *prev;
	void *start_heap;
	void *brk;
	struct mem_map *mem_map;

	pid_t pid;
};

struct mem_map {
	struct page_struct *phys_page;
	intptr_t virt_addr;
	size_t map_size;
	struct mem_map *next;
	struct mem_map *prev;
};

#endif
