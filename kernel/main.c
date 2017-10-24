/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

void
kmain(void)
{
	/* Print 'Ok' on screen */
	*(unsigned int *)0xB8000 = 0x2f4b2f4f;

	/* Halt */
	while (42);
}
