/*
 *
 *   kmemcpy.c
 *   kmemcpy function for the kernel
 *
 *
 */

#include <stddef.h>

#include <kernel/lib/kmemcpy.h> 

void* kmemcpy(void *dest, void *src, size_t num)
{
	char *dest8 = (char *)dest;
	char *src8 = (char *)src;
	for (int i = 0; i < num; i++) {
		src8[i] = dest8[i];
	}
	return dest;
}
