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
# include <stdarg.h>

/*
** A directory entry, as returned by the readdir syscall.
*/
struct dirent
{
	char name[256];
	bool dir;
};

/*
** This file is a microscopic C library with syscall wrappers. It's here to
** help writting userspace programs.
*/

/* Syscalls */
__unused
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

__unused
__attribute__ ((noinline))
static status_t
clone(void (*cb)(void))
{
	return (syscall(SYSCALL_CLONE, (uintptr)cb, 0, 0, 0));
}

__unused
__attribute__ ((noinline))
__noreturn
static void
exit(uchar status)
{
	syscall(SYSCALL_EXIT, status, 0, 0, 0);
	__builtin_unreachable();
}

__unused
__attribute__ ((noinline))
static status_t
exec(char const *path)
{
	return (syscall(SYSCALL_EXEC, (uintptr)path, 0, 0, 0));
}

__unused
__attribute__ ((noinline))
static file_handle_t
open(char const *path)
{
	return (syscall(SYSCALL_OPEN, (uintptr)path, 0, 0, 0));
}

__unused
__attribute__ ((noinline))
static ssize_t
read(file_handle_t handle, void *str, size_t n)
{
	return (syscall(SYSCALL_READ, handle, (uintptr)str, n, 0));
}

__unused
__attribute__ ((noinline))
static ssize_t
write(file_handle_t handle, void const *str, size_t n)
{
	return (syscall(SYSCALL_WRITE, handle, (uintptr)str, n, 0));
}

__unused
__attribute__ ((noinline))
static status_t
close(file_handle_t h)
{
	return (syscall(SYSCALL_CLOSE, h, 0, 0, 0));
}

__unused
__attribute__ ((noinline))
static dir_handle_t
opendir(char const *path)
{
	return (syscall(SYSCALL_OPENDIR,(uintptr)path, 0, 0, 0));
}

__unused
__attribute__ ((noinline))
static char
keyboard(void)
{
	return (syscall(SYSCALL_KEYBOARD, 0, 0, 0, 0));
}

__unused
__attribute__ ((noinline))
static status_t
readdir(dir_handle_t h, struct dirent *dirent)
{
	return (syscall(SYSCALL_READDIR, h, (uintptr)dirent, 0, 0));
}

__unused
__attribute__ ((noinline))
static status_t
closedir(dir_handle_t h)
{
	return (syscall(SYSCALL_CLOSEDIR, h, 0, 0, 0));
}

/* C Library */

__unused
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

__unused
static void
putc(char c)
{
	write(0, &c, 1);
}

__unused
static void
putstr(char const *str)
{
	write(0, str, strlen(str));
}

__unused
static char
strcmp(char const *s1, char const *s2)
{
	while (*s1 == *s2) {
		if (*s1 == '\0') {
			return (0);
		}
		++s1;
		++s2;
	}
	return (s1 - s2);
}

__unused
static bool
is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

__unused
static void
putnbr(long i) // Quick & dirty
{
	if (i < 0) {
		putc('-');
		i *= -1;
	}
	if (i >= 10) {
		putnbr(i / 10);
	}
	putc('0' + i % 10);
}

__unused
static void
putptr(unsigned long i) // Quick & dirty
{
	if (i >= 16) {
		putnbr(i / 16);
	}
	write(1, "0123456789abcdef" + i % 16, 1);
}

__unused
static void
printf(char const *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	while (*fmt) {
		if (*fmt == '%') {
			++fmt;
			switch (*fmt) {
			case 'c':
				putc(va_arg(va, int));
				break;
			case 's':
				putstr(va_arg(va, char *));
				break;
			case 'i': case 'd':
				putnbr(va_arg(va, int));
				break;
			case 'p':
				putstr("0x");
				putptr((unsigned long)va_arg(va, void *));
				break;
			case 'u':
				putnbr(va_arg(va, unsigned int));
				break;
			}
		} else {
			putc(*fmt);
		}
		++fmt;
	}
	va_end(va);
}

#endif /* !_USERSPACE_ULIB_H_ */
