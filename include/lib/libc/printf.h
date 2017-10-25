/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _LIBC_PRINTF_H_
# define _LIBC_PRINTF_H_

# include <stddef.h>
# include <stdarg.h>

# define PRINTF_HALF		0x00000001
# define PRINTF_HALFHALF	0x00000002
# define PRINTF_LONG		0x00000004
# define PRINTF_SIZE_T		0x00000008
# define PRINTF_PTRDIFF_T	0x00000010

# define PRINTF_CAPS		0x00000020
# define PRINTF_SHOW_SIGN	0x00000040
# define PRINTF_SIGNED_INT	0x00000080
# define PRINTF_SHOW_BASE	0x00000100
# define PRINTF_LEFT_FORMAT	0x00000200
# define PRINTF_LEAD_ZEROS	0x00000400
# define PRINTF_BLANK		0x00000800

typedef	int(*printf_output_func)(char const *, size_t, void *);

int	printf_formatter(char const *fmt, va_list va, printf_output_func, void *params);

#endif /* !_LIBC_PRINTF_H_ */
