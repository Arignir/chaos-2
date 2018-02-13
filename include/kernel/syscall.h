/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_SYSCALL_H_
# define _KERNEL_SYSCALL_H_

# include <chaosdef.h>
# include <chaoserr.h>

enum syscall_id
{
	SYSCALL_UNKNOWN		= 0,
	SYSCALL_CLONE		= 1,
	SYSCALL_EXIT		= 2,
	SYSCALL_EXEC		= 3,
	SYSCALL_WRITE		= 4,
};

/*
** Provides implementation for all given syscalls.
*/

status_t	sys_clone(void *main);
void		sys_exit(uchar status);
status_t	sys_exec(char const *path);
int		sys_write(file_handle_t handle, char const *buff, size_t s);

#endif /* !_KERNEL_SYSCALL_H_ */
