/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/hexdump.h>
#include <stdio.h>

static void
hexdump_char(uchar const *start, size_t len)
{
	size_t i = 0;

	while (i < len) {
		printf("%02x ", start[i]);
		++i;
	}
	while (i < HEXDUMP_WIDTH) {
		printf("   ", start[i]);
		++i;
	}
}

static void
hexdump_str(uchar const *start, size_t len)
{
	size_t i = 0;
	char c;

	while (i < len) {
		c = start[i];
		if (c >= ' ' && c <= '~') { /* is_printable() */
			printf("%c", c);
		} else {
			printf(".");
		}
		++i;
	}
	while (i < HEXDUMP_WIDTH) {
		printf(" ");
		++i;
	}
}

/*
** Dumps the given memory
*/
void
hexdump(uchar const *start, size_t size)
{
	uchar const *end;
	size_t len;

	end = start + size;
	while (start < end)
	{
		len = end - start;
		if (len >= HEXDUMP_WIDTH) {
			len = HEXDUMP_WIDTH;
		}
		printf("%p ", start);
		hexdump_char(start, len);
		printf("|");
		hexdump_str(start, len);
		printf("|\n");
		start += HEXDUMP_WIDTH;
	}
}
