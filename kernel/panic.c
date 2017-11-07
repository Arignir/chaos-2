/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdarg.h>

/*
** Produces a kernel panic, printing the given error message and some debug
** informations.
** Never returns.
*/
void
panic(char const *fmt, ...)
{
	va_list va;

	// TODO: Disable interrupts
	va_start(va, fmt);
	printf("\nKernel panicked: ");
	vprintf(fmt, va);
	va_end(va);

	while (42);
}
