#ifndef _MM_H_
#define _MM_H_

#include <stddef.h>
#include <stdint.h>

/*
 *  size must be 2-byte alligned, excludes header size.
 *  LSB of size represents allocation status, 1 being allocated
 */

#define HEADER_SIZE (sizeof(struct bh))

void mminit();

void init_heap(void *heap_base);

void add_heap(void *heap_base);

void *kmalloc(size_t bytes);
#endif
