#ifndef _PIT_C_
#define _PIT_C_

#include <stdint.h>
#include <stdbool.h>

extern bool pit_oneshot_done;

void prime_pit(uint32_t time);

void fire_pit();

void square_pit();

void oneshot_handler();

static inline bool is_oneshot_done()
{
	return pit_oneshot_done;
}
#endif
