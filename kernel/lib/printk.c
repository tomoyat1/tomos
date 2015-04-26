/*
 *
 *  kprint.c
 *  kprint(char* string) ... print string to screen.
 *
 *
 */


#include <stdint.h>
#include <stddef.h>

#include <kernel/printk.h>
#include <kernel/strlen.h>

#include <kernel/vga.h>


void printk(char* string)
{
	for ( size_t i = 0; i < strlen(string); i++ )
		{
			write_char(string[i]);
		}
}
