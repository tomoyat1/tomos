/*
 * kstrcat.c
 * concatenate strings. Assumes that *dest has enough space.
 */

#include "kstrcat.h"

#include <stddef.h>

//TODO: Implement this without depending on kstrcat.
char *kstrcat(char *dest, const char *src)
{
	size_t dest_end;

	for (dest_end = 0; ; dest_end++) {
		if ( dest[dest_end] == '\0')
			break;
	}

	//Gotta append \0 at the end.
	int i;
	for (i = 0; src[i] != '\0' ; i++)
		dest[i + dest_end] = src[i];
	dest[i + dest_end] = '\0';
	return dest;
}
