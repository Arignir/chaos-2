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
#include <kernel/vmm.h>
#include <drivers/vga.h>

/*
** Creates a new thread starting at the given function.
*/
status_t
sys_clone(void *main)
{
	status_t s;

	s = thread_clone(main);
	return (s);
}

/*
** Exits the current thread
*/
__noreturn
void
sys_exit(uchar status)
{
	current_thread_acquire_write();
	current_vaspace_acquire_write();
	thread_exit(status);
}

/*
** Executes the given binary, replacing the current vaspace by a new one.
*/
status_t
sys_exec(char const *path)
{
	status_t s;

	current_thread_acquire_write();
	current_vaspace_acquire_write();
	if (vmm_validate_str(path, NULL)) {
		thread_exit(EXIT_PAGEFAULT);
	}
	s = thread_exec(path);
	current_vaspace_release_write();
	current_thread_release_write();
	return (s);
}

/*
** Writes the given buffer to the given file.
*/
int
sys_write(file_handle_t handle __unused /* TODO */, char const *buff, size_t s)
{
	if (vmm_validate_buffer((uchar const *)buff, s)) {
		thread_exit(EXIT_PAGEFAULT);
	}
	return (vga_putsn(buff, s));
}
