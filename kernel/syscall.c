/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/syscall.h>
#include <kernel/thread.h>
#include <kernel/vaspace.h>
#include <drivers/vga.h>

/*
** Creates a new thread starting at the given function.
*/
status_t
sys_clone(void *main)
{
	/* TODO FIXME We should ensure 'main' points to userspace memory */

	struct vaspace *vaspace;
	status_t s;

	vaspace = current_vaspace();
	rwlock_acquire_write(&vaspace->rwlock);
	s = thread_clone(main);
	rwlock_release_write(&vaspace->rwlock);
	return (s);
}

/*
** Writes the given buffer to the given file.
*/
int
sys_write(file_handler_t handler __unused, char const *buff, size_t s)
{
	/* TODO FIXME We should ensure 'buff' points to userspace memory. */
	return (vga_putsn(buff, s));
}
