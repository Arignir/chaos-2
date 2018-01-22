/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _USERSPACE_ULIB_H_
# define _USERSPACE_ULIB_H_

/* Some little kernel dependencies (header-only) to make things a bit easier. */
# include <chaosdef.h>
# include <chaoserr.h>
# include <kernel/syscall.h>

/*
** This file is a microscopic C library with syscall wrappers. It's here to
** help writting userspace programs.
*/

/* Syscalls */
static inline uintptr
syscall(uintptr eax, uintptr edi, uintptr esi, uintptr edx, uintptr ecx)
{
	uintptr ret;

	asm volatile(
		"int $0x80\n\t"
		: "=a"(ret) : "a"(eax), "D"(edi), "S"(esi), "d"(edx), "c"(ecx)
	);
	return (ret);
}

static status_t
clone(void (*cb)(void))
{
	return (syscall(SYSCALL_CLONE, (uintptr)cb, 0, 0, 0));
}

static size_t
write(file_handler_t handler, char const *str, size_t n)
{
	return (syscall(SYSCALL_WRITE, handler, (uintptr)str, n, 0));
}

/* C Library */

static size_t strlen(char const *str)
{
	size_t i;

	i = 0;
	while (*str) {
		++str;
		++i;
	}
	return (i);
}

static void
putstr(char const *str)
{
	write(0, str, strlen(str));
}

#endif /* !_USERSPACE_ULIB_H_ */
