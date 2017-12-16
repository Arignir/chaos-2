/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/interrupts.h>
#include <stdio.h>
#include <stdarg.h>

/*
** Produces a kernel panic, printing the given error message and some debug
** informations.
** Never returns.
**
** This function must be self-contained as much as possible, as we don't know the state
** of the system when it will be called.
*/
void
panic(char const *fmt, ...)
{
	va_list va;

	disable_interrupts();

	va_start(va, fmt);
	printf("\nKernel panicked: ");
	vprintf(fmt, va);
	va_end(va);

	while (42);
}
