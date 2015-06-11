#ifndef _PROC_H_
#define _PROC_H_

#include <kernel/mm.h>

/* Proc struct. should be placed directly past top of stack. */
struct proc_struct;
struct proc_struct {
	struct bh *proc_heap;
	struct proc_struct *next;
};

/* 
 * Page tables need to be 4KiB aligned 
 * 
 */
#endif
