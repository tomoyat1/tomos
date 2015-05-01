#ifndef _MM_H_
#define _MM_H_

#include <stddef.h>
#include <stdint.h>

#define CHUNK_SIZE (size_t)0x10
#define BYTES_TO_CHUNKS(bytes) (bytes / CHUNK_SIZE + 1)
typedef struct free_mem free_mem_t;

typedef struct free_mem
{
	size_t size;
	char *start_addr;
	free_mem_t *next;
	free_mem_t *prev;
} free_mem_t;


void mminit();

free_mem_t* search_free_mem(size_t chunks);

void* get_free_mem_from_block(free_mem_t* mem, size_t bytes);
#endif
