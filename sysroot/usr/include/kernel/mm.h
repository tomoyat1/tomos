#ifndef _MM_H_
#define _MM_H_

#include <stddef.h>
#include <stdint.h>

typedef struct free_mem free_mem_t;

typedef struct free_mem
{
	size_t size;
	char* start_addr;
	free_mem_t* next;
	free_mem_t* prev;
} free_mem_t;


void mminit();

free_mem_t* search_free_mem(size_t bytes);
#endif
