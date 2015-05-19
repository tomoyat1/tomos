#ifndef _MM_H_
#define _MM_H_

#include <stddef.h>
#include <stdint.h>

typedef struct bh bh_t;
//bh means block_header. Should be put at the front of each page.
typedef struct bh 
{
	size_t size; /*Must be 2-byte alligned, excludes header size*/
	bh_t *next;
} bh_t;
/*
 *  size must be 2-byte alligned, excludes header size.
 *  LSB of size represents allocation status, 1 being allocated
 */

#define HEADER_SIZE (sizeof(bh_t))

void mminit(void *heap_base);

//return start of free region.
void *alloc_free(size_t demand_size, void *heap_base);

void free_allocated(void *addr, void *heap_base);

#endif
