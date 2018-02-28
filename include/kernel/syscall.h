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

struct dirent;

enum syscall_id
{
	SYSCALL_UNKNOWN		= 0,
	SYSCALL_CLONE,
	SYSCALL_EXIT,
	SYSCALL_EXEC,
	SYSCALL_OPEN,
	SYSCALL_READ,
	SYSCALL_WRITE,
	SYSCALL_CLOSE,
	SYSCALL_OPENDIR,
	SYSCALL_READDIR,
	SYSCALL_CLOSEDIR,
	SYSCALL_KEYBOARD,
};

/*
** Provides implementation for all given syscalls.
*/

status_t	sys_clone(void *main);
void		sys_exit(uchar status);
status_t	sys_exec(char const *path);
file_handle_t	sys_open(char const *path);
ssize_t		sys_read(file_handle_t handle, char *buff, size_t s);
ssize_t		sys_write(file_handle_t handle, char const *buff, size_t s);
status_t	sys_close(file_handle_t handle);
dir_handle_t	sys_opendir(char const *path);
status_t	sys_readdir(dir_handle_t, struct dirent *dirent);
status_t	sys_closedir(dir_handle_t dir);
char		sys_keyboard(void);

#endif /* !_KERNEL_SYSCALL_H_ */
