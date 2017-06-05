#ifndef _PAGE_FAULT_H_
#define _PAGE_FAULT_H_

#include <stddef.h>
#include <stdint.h>

void page_fault_handler(intptr_t virt_addr);

#endif
