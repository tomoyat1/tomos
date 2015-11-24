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
#include <kernel/drivers/vga.h>


void printk(char *string)
{
	for (size_t i = 0; i < strlen(string); i++) {
			write_char(string[i]);
	}
}

/* debugging functions */

void printint(int n)
{
	char nums[10] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};
	char buffer[100] = {'\0'};

	int tmp = n;
	for (int i = 0; tmp != 0; i++) {
		buffer[i] = nums[tmp % 10];
		tmp = tmp / 10;
	}
	
	for (int i = 0; buffer[i] != '\0'; i++) {
		write_char(buffer[i]);
	}
}

void printaddr(int n)
{
	char nums[16] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b',\
			'c', 'd', 'e'
	};
	char buffer[100] = {'\0'};

	int tmp = n;
	for (int i = 0; tmp != 0; i++) {
		buffer[i] = nums[tmp % 16];
		tmp = tmp / 16;
	}
	
	for (int i = 0; buffer[i] != '\0'; i++) {
		write_char(buffer[i]);
	}
	write_char('x');
	write_char('0');
}
