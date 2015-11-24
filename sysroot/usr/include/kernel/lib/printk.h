#ifndef _PRINTK_H_
#define _PRINTK_H_

#include <kernel/drivers/vga.h>

void printk(char* string);

void printint(int n);

void printaddr(int n);
#endif
