#ifndef _MM_H_
#define _MM_H_

#include <stddef.h>
#include <stdint.h>

#define CHUNK_SIZE (size_t)0x10
#define BYTES_TO_CHUNKS(bytes) (bytes / CHUNK_SIZE + 1)

void mminit();

typedef struct bitmap_for_page bitmap_for_page_t;
//Should be put at the front of each page.
typedef struct bitmap_for_page
{
	uint32_t bitmap[128];
	bitmap_for_page_t *next_page;
} bitmap_for_page_t;

void mminit();

//return start of free region.
void* alloc_free(size_t size, void *heap_base);

void free_allocated(void *addr, void *heap_base);

#endif
