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
	//Gotta append \0 at end
	for (i = 0; s2[i] != '\0'; i++)
		dest[i] = src[i];
	dest[i] = '\0';
	return dest;
}

