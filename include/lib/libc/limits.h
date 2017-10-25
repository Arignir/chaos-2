/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _LIBC_LIMITS_H_
# define _LIBC_LIMITS_H_

# include <bits/limits.h>

# define CHAR_MIN		(-128)
# define CHAR_MAX		(127)
# define UCHAR_MAX		(255U)
# define SHRT_MIN		(-1 - SHRT_MAX)
# define SHRT_MAX		(0x7FFF)
# define USHRT_MAX		(0xFFFFU)
# define INT_MIN		(-1 - INT_MAX)
# define INT_MAX		(0x7FFFFFFF)
# define UINT_MAX		(0xFFFFFFFFU)
# define LONG_MIN		(-1 - LONG_MAX)
# define LONG_MAX		(0x7FFFFFFFFFFFFFFF)
# define ULONG_MAX		(0xFFFFFFFFFFFFFFFFUL)

# define PAGE_SIZE		(4096)

#endif /* !_LIBC_LIMITS_H_ */
