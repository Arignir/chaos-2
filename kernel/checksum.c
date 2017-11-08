/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/checksum.h>

uint8
checksum8(void const *s, size_t l)
{
	uint8 r;
	unsigned char const *c;
	unsigned char const *e;

	r = 0;
	c = s;
	e = s + l;
	while (c < e) {
		r += *c;
		++c;
	}
	return (r);
}

uint16
checksum16(void const *s, size_t l)
{
	uint16 r;
	unsigned char const *c;
	unsigned char const *e;

	r = 0;
	c = s;
	e = s + l;
	while (c < e) {
		r += *c;
		++c;
	}
	return (r);
}

uint32
checksum32(void const *s, size_t l)
{
	uint32 r;
	unsigned char const *c;
	unsigned char const *e;

	r = 0;
	c = s;
	e = s + l;
	while (c < e) {
		r += *c;
		++c;
	}
	return (r);
}
