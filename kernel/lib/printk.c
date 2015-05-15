/*
 *
 *  kprint.c
 *  kprint(char* string) ... print string to screen.
 *
 *
 */


#include <stdint.h>
#include <stddef.h>

#include <kernel/lib/printk.h>

#include <kernel/kstring.h>
#include <kernel/vga.h>


void printk(char *string)
{
	for (size_t i = 0; i < strlen(string); i++) {
			write_char(string[i]);
	}
}
