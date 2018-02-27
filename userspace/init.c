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
int main(void) {
	putstr("Hello Userspace World!\n");

	while (42) {
		putstr(".");
		for (unsigned int i = 0; i < 100000000; ++i);
	}
	return (0);
}
