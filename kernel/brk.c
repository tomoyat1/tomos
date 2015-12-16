/*
    brk.c
    brk system call
*/
#include <stddef.h>
#include <stdint.h>

#include <kernel/brk.h>
#include <kernel/proc.h>

extern struct proc_struct *kernel_thread;

void * brk(void *addr)
{
	/*
	   Hardcode to kernel break for now.
	   Get current process from sched in the future.
	   (Current running process does NOT change in system calls)
	*/
	if ((intptr_t)addr < (intptr_t)(kernel_thread->brk))
		return NULL;

	kernel_thread->brk = addr;
	return kernel_thread->brk;
}

void * sbrk(intptr_t increment)
{
	intptr_t newbrk = (intptr_t)(kernel_thread->brk) + increment;
	return brk((void *)newbrk);
}
