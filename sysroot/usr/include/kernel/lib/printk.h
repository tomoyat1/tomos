#ifndef _PRINTK_H_
#define _PRINTK_H_

#include <kernel/drivers/vga.h>

void printk(char* string);

void
printdec(unsigned int i);

void
printhex(unsigned int i);

#endif
