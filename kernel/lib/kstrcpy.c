/*
 *  kstrcpy.c
 *  Copy string from a to b
 */

#include <kernel/lib/kstrcpy.h>

char *kstrcpy(char *s1, const char *s2)
{
	char *dest = s1;
	const char *src = s2;
	int i = 0;
	while ( s2[i] != '\0' )
	{
		dest[i] = src[i];
		i++;
	}
	return dest;
}

