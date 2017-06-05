/*
 *
 *  kprint.c
 *  kprint(char* string) ... print string to screen.
 *
 * 1
 */


#include <stdint.h>
#include <stddef.h>

#include <kernel/lib/printk.h>

#include <kernel/kstring.h>
#include <kernel/drivers/vga.h>

char * itoa(unsigned int i, char *str, int radix)
{
	char table[16] = {
		'0', '1', '2', '3', \
		'4', '5', '6', '7', \
		'8', '9', 'a', 'b', \
		'c', 'd', 'e', 'f' \
	};
	unsigned int j = i;
	unsigned int digits = 0;

	do {
		digits++;
	} while (j /= radix);
	str[digits] = '\0';
	j = digits - 1;
	do {
		str[j] = table[i % radix];
		j--;
	} while (i /= radix);
	return (str);
}

void printk(char *string)
{
	for (size_t i = 0; i < strlen(string); i++) {
			write_char(string[i]);
	}
}

void
printdec(unsigned int i)
{
	char buf[64];
	itoa(i, buf, 10);
	printk(buf);
	printk("\n");
}

void
printhex(unsigned int i)
{
	char buf[64];
	itoa(i, buf, 16);
	printk(buf);
	printk("\n");
}
