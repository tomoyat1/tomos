#ifndef _VM_H_
#define _VM_H_

#include <stddef.h>

#include <kernel/proc.h>

void page_fault_handler(intptr_t virt_addr);

struct mem_map * map_phys(struct page_struct *ps, intptr_t virt_addr);

#endif
