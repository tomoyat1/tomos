#include <kernel/timer.h>

#include <kernel/x86/interrupts.h>
#include <kernel/x86/pit.h>

#include <kernel/klib.h>

#include <stdint.h>

/* System clock in miliseconds */
uint32_t sys_clock = 0;

void inc_sys_clock()
{
	sys_clock += 1;
	pic_eoi();
}

/* TODO: Eventually make this a sched function */
void wait(uint32_t time)
{
	sys_clock = 0;
	uint32_t end_time = sys_clock + time;
	while (sys_clock < end_time) {
		continue;
	}
}
