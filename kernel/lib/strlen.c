/*
 *
 *  strlen.c
 *  Returns length of string.
 *
 *
 */


#include <stddef.h>

#include <kernel/strlen.h>

size_t strlen(char* string)
{
	size_t len = 0;
	while ( string[len] != 0 )
	{
		len++;
	}
	return len;
}


