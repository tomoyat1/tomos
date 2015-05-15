/*
 *  kstrcmp.c
 *  compares two strings, should work as POSIX says. 
 *
 */

#include <kernel/lib/kstrcmp.h>

int kstrcmp(const char * str1, const char * str2)
{
	int index;
	for ( index = 0; str1[index] != '\0' || str2[index] != '\0'; index++ )
	{
		if ( str1[index] != str2[index] )
		{
			return (int)str1[index] - (int)str2[index];
		}
	}
	if ( str1[index] == '\0' && str2[index] == '\0' )
	{
		return 0;
	}
	return (int)str1[index] - (int)str2[index];
}
