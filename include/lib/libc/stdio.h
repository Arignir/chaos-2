/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _LIBC_STDIO_H_
# define _LIBC_STDIO_H_

# include <chaosdef.h>
# include <stdarg.h>

struct vsnprintf_output_args
{
	char *str;
	size_t len;
	size_t pos;
};

struct printf_output_args
{
	int ret;
};

int		puts(char const *str);
int		putsn(char const *str, size_t); /* Not standard but useful */
int		putchar(int c);

/* printf-like functions */
int		printf(char const *fmt, ...);
int		sprintf(char *str, char const *fmt, ...);
int		snprintf(char *str, size_t size, char const *fmt, ...);

/* vprintf-like functions */
int		vprintf(char const *fmt, va_list va);
int		vsprintf(char *str, char const *fmt, va_list va);
int		vsnprintf(char *str, size_t size, char const *fmt, va_list va);

#endif /* !_LIBC_STDIO_H_ */
