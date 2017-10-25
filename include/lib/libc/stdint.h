/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _LIBC_STDINT_H_
# define _LIBC_STDINT_H_

# define __NEED_INT8_T
# define __NEED_INT16_T
# define __NEED_INT32_T
# define __NEED_INT64_T
# define __NEED_UINT8_T
# define __NEED_UINT16_T
# define __NEED_UINT32_T
# define __NEED_UINT64_T

# define __NEED_INT_FAST8_T
# define __NEED_INT_FAST16_T
# define __NEED_INT_FAST32_T
# define __NEED_INT_FAST64_T
# define __NEED_UINT_FAST8_T
# define __NEED_UINT_FAST16_T
# define __NEED_UINT_FAST32_T
# define __NEED_UINT_FAST64_T

# define __NEED_INT_LEAST8_T
# define __NEED_INT_LEAST16_T
# define __NEED_INT_LEAST32_T
# define __NEED_INT_LEAST64_T
# define __NEED_UINT_LEAST8_T
# define __NEED_UINT_LEAST16_T
# define __NEED_UINT_LEAST32_T
# define __NEED_UINT_LEAST64_T

# define __NEED_INTPTR_T
# define __NEED_UINTPTR_T
# define __NEED_INTMAX_T
# define __NEED_UINTMAX_T

# include <bits/types.h>

#endif /* !_LIBC_STDINT_H_ */
