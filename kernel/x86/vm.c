/*
 *  vm.c
 *  Stuff about x86 virtual memory
 */

#include <kernel/mm.h>
#include <kernel/x86/vm.h>

extern struct proc_struct *current;

/* Actual changing of page tables */
struct mem_map * map_phys(struct page_struct *ps, intptr_t virt_addr)
{
	struct mem_map *m = current->mem_map;
	if (m) {
		m = current->mem_map;
		while (m->next) {
			if (m->phys_page->phys_addr + m->map_size == ps->phys_addr) {
				m->map_size += 0x1000;
				goto MAP;
			} else {
				m = m->next;
			}
		}
		m->next = kmalloc(sizeof(struct mem_map));
	} else {
		m = kmalloc(sizeof(struct mem_map));
	}
	m->map_size = 0x1000;

/* Editing of PTE and TLB busting */
MAP:

	return (m);
}
