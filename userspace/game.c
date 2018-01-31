/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <ulib.h>
#include <arch/x86/asm.h>

__optimize(0)
__unused
static bool
are_interrupts_enabled(void)
{
	return (get_eflags().IF);
}

__optimize(0)
static void
thread2(void)
{
	putstr("Hello Userspace World!\n");
	while (42) {
		putstr("o");
		for (int i = 0; i < 100000000; ++i); // "Sleep" for the poors
	}
}

__optimize(0)
int main(void) {
	putstr("Hello Userspace World!\n");
	clone(&thread2);
	while (42) {
		putstr(".");
		for (int i = 0; i < 100000000; ++i); // "Sleep" for the poors
	}
	return (0);
}
