/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _LIBC_STDDEF_H_
# define _LIBC_STDDEF_H_

# define NULL			((void *)0u)
# define PAGE_SIZE		(4096u)

# define __NEED_SIZE_T
# define __NEED_PTRDIFF_T

# include <bits/types.h>

# define offsetof(t, m)		__builtin_offsetof(t, m)

#endif /* !_LIBC_STDDEF_H_ */
