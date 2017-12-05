/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <arch/x86/timer.h>
#include <arch/x86/asm.h>
#include <stdio.h> // TODO Debug

# define MS_PER_TICK 100

static uint64 time_at_last_tick = 0;
static uint64 time_per_tick = 0;
static uint64 timestamp = 0;

/*
** Returns the current timestamp
*/
uint64
now(void)
{
	uint64 tsc = rdtsc();
	tsc -= time_at_last_tick;
	tsc *= MS_PER_TICK;
	if (time_per_tick) {
		tsc /= time_per_tick;
	} else {
		tsc = 0;
	}
	return (timestamp + tsc);
}

/*
** Called by the timer interrupt handler
*/
void
timer_ihandler(void)
{
	uint64 tsc = rdtsc();

	if (time_at_last_tick) {
		time_per_tick = tsc - time_at_last_tick;
	}
	time_at_last_tick = tsc;
	printf("Now? %zu\n", now());
}
