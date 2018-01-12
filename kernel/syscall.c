/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/syscall.h>
#include <drivers/vga.h>

/*
** Writes the given buffer to the given file.
*/
int
sys_write(file_handler_t handler __unused, char const *buff, size_t s)
{
	/* TODO FIXME We should ensure the 'buff' pointer belongs to userspace memory. */
	return (vga_putsn(buff, s));
}
