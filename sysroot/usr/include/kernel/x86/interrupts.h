#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_h_

#include <kernel/x86/port.h>

struct tss_struct
{
	uint32_t prev_task_link;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt_selector;
	uint32_t io_map_base;
} __attribute__((packed));

void set_tss();

void set_idt();

static inline void pic_eoi()
{
	outb(0x20, 0x20);
}

void divzero();

void double_fault_panic();
#endif
