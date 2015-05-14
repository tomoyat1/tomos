/*
 * pow.c
 * power function
 */

#include <kernel/lib/pow.h>

int pow(int x, int y)
{
	if ( y == 0 )
	{
		return 1;
	}
	else
	{
		int result = 1;
		for ( int i = 1; i <= y; i++ )
		{
			result *=x;
		}
		return result;
	}


}

