/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _LIBC_BITS_TYPES_H_
# define _LIBC_BITS_TYES_H_

# if defined(__NEED_VA_LIST) && !defined(__DEFINED_VA_LIST)
typedef __builtin_va_list	va_list;
#  define __DEFINED_VA_LIST
# endif /* !__NEED_VA_LIST */

# if defined(__NEED_SIZE_T) && !defined(__DEFINED_SIZE_T)
typedef __SIZE_TYPE__		size_t;
#  define __DEFINED_SIZE_T
# endif /* !__NEED_SIZE_T */

# if defined(__NEED_PTRDIFF_T) && !defined(__DEFINED_PTRDIFF_T)
typedef __PTRDIFF_TYPE__	ptrdiff_t;
#  define __DEFINED_PTRDIFF_T
# endif /* !__NEED_PTRDIFF_T */

# if defined(__NEED_INTPTR_T) && !defined(__DEFINED_INTPTR_T)
typedef __INTPTR_TYPE__		intptr_t;
#  define __DEFINED_INTPTR_T
# endif /* !__NEED_INTPTR_T */

# if defined(__NEED_UINTPTR_T) && !defined(__DEFINED_UINTPTR_T)
typedef __UINTPTR_TYPE__	uintptr_t;
#  define __DEFINED_UINTPTR_T
# endif /* !__NEED_UINTPTR_T */

# if defined(__NEED_INTMAX_T) && !defined(__DEFINED_INTMAX_T)
typedef __INTMAX_TYPE__		intmax_t;
#  define __DEFINED_INTMAX_T
# endif /* !__NEED_INTMAX_T */

# if defined(__NEED_UINTMAX_T) && !defined(__DEFINED_UINTMAX_T)
typedef __UINTMAX_TYPE__	uintmax_t;
#  define __DEFINED_UINTMAX_T
# endif /* !__NEED_UINTMAX_T */

/* ===		signed integer types		=== */

# if defined(__NEED_INT8_T) && !defined(__DEFINED_INT8_T)
typedef __INT8_TYPE__		int8_t;
#  define __DEFINED_INT8_T
# endif /* !__NEED_INT8_T */

# if defined(__NEED_INT16_T) && !defined(__DEFINED_INT16_T)
typedef __INT16_TYPE__		int16_t;
#  define __DEFINED_INT16_T
# endif /* !__NEED_INT16_T */

# if defined(__NEED_INT32_T) && !defined(__DEFINED_INT32_T)
typedef __INT32_TYPE__		int32_t;
#  define __DEFINED_INT32_T
# endif /* !__NEED_INT32_T */

# if defined(__NEED_INT64_T) && !defined(__DEFINED_INT64_T)
typedef __INT64_TYPE__		int64_t;
#  define __DEFINED_INT64_T
# endif /* !__NEED_INT64_T */

# if defined(__NEED_UINT8_T) && !defined(__DEFINED_UINT8_T)
typedef __UINT8_TYPE__		uint8_t;
#  define __DEFINED_UINT8_T
# endif /* !__NEED_UINT8_T */

# if defined(__NEED_UINT16_T) && !defined(__DEFINED_UINT16_T)
typedef __UINT16_TYPE__		uint16_t;
#  define __DEFINED_UINT16_T
# endif /* !__NEED_UINT16_T */

# if defined(__NEED_UINT32_T) && !defined(__DEFINED_UINT32_T)
typedef __UINT32_TYPE__		uint32_t;
#  define __DEFINED_UINT32_T
# endif /* !__NEED_UINT32_T */

# if defined(__NEED_UINT64_T) && !defined(__DEFINED_UINT64_T)
typedef __UINT64_TYPE__		uint64_t;
#  define __DEFINED_UINT64_T
# endif /* !__NEED_UINT64_T */

/* ===			end			=== */
/* ===		fast integer types		=== */

# if defined(__NEED_INT_FAST8_T) && !defined(__DEFINED_INT_FAST8_T)
typedef __INT_FAST8_TYPE__		int_fast8_t;
#  define __DEFINED_INT_FAST8_T
# endif /* !__NEED_INT_FAST8_T */

# if defined(__NEED_INT_FAST16_T) && !defined(__DEFINED_INT_FAST16_T)
typedef __INT_FAST16_TYPE__		int_fast16_t;
#  define __DEFINED_INT_FAST16_T
# endif /* !__NEED_INT_FAST16_T */

# if defined(__NEED_INT_FAST32_T) && !defined(__DEFINED_INT_FAST32_T)
typedef __INT_FAST32_TYPE__		int_fast32_t;
#  define __DEFINED_INT_FAST32_T
# endif /* !__NEED_INT_FAST32_T */

# if defined(__NEED_INT_FAST64_T) && !defined(__DEFINED_INT_FAST64_T)
typedef __INT_FAST64_TYPE__		int_fast64_t;
#  define __DEFINED_INT_FAST64_T
# endif /* !__NEED_INT_FAST64_T */

# if defined(__NEED_UINT_FAST8_T) && !defined(__DEFINED_UINT_FAST8_T)
typedef __UINT_FAST8_TYPE__		uint_fast8_t;
#  define __DEFINED_UINT_FAST8_T
# endif /* !__NEED_UINT_FAST8_T */

# if defined(__NEED_UINT_FAST16_T) && !defined(__DEFINED_UINT_FAST16_T)
typedef __UINT_FAST16_TYPE__		uint_fast16_t;
#  define __DEFINED_UINT_FAST16_T
# endif /* !__NEED_UINT_FAST16_T */

# if defined(__NEED_UINT_FAST32_T) && !defined(__DEFINED_UINT_FAST32_T)
typedef __UINT_FAST32_TYPE__		uint_fast32_t;
#  define __DEFINED_UINT_FAST32_T
# endif /* !__NEED_UINT_FAST32_T */

# if defined(__NEED_UINT64_T) && !defined(__DEFINED_UINT64_T)
typedef __UINT64_TYPE__			uint_fast64_t;
#  define __DEFINED_UINT64_T
# endif /* !__NEED_UINT64_T */

/* ===			end			=== */
/* ===		least integer types		=== */

# if defined(__NEED_INT_LEAST8_T) && !defined(__DEFINED_INT_LEAST8_T)
typedef __INT_LEAST8_TYPE__		int_least8_t;
#  define __DEFINED_INT_LEAST8_T
# endif /* !__NEED_INT_LEAST8_T */

# if defined(__NEED_INT_LEAST16_T) && !defined(__DEFINED_INT_LEAST16_T)
typedef __INT_LEAST16_TYPE__		int_least16_t;
#  define __DEFINED_INT_LEAST16_T
# endif /* !__NEED_INT_LEAST16_T */

# if defined(__NEED_INT_LEAST32_T) && !defined(__DEFINED_INT_LEAST32_T)
typedef __INT_LEAST32_TYPE__		int_least32_t;
#  define __DEFINED_INT_LEAST32_T
# endif /* !__NEED_INT_LEAST32_T */

# if defined(__NEED_INT_LEAST64_T) && !defined(__DEFINED_INT_LEAST64_T)
typedef __INT_LEAST64_TYPE__		int_least64_t;
#  define __DEFINED_INT_LEAST64_T
# endif /* !__NEED_INT_LEAST64_T */

# if defined(__NEED_UINT_LEAST8_T) && !defined(__DEFINED_UINT_LEAST8_T)
typedef __UINT_LEAST8_TYPE__		uint_least8_t;
#  define __DEFINED_UINT_LEAST8_T
# endif /* !__NEED_UINT_LEAST8_T */

# if defined(__NEED_UINT_LEAST16_T) && !defined(__DEFINED_UINT_LEAST16_T)
typedef __UINT_LEAST16_TYPE__		uint_least16_t;
#  define __DEFINED_UINT_LEAST16_T
# endif /* !__NEED_UINT_LEAST16_T */

# if defined(__NEED_UINT_LEAST32_T) && !defined(__DEFINED_UINT_LEAST32_T)
typedef __UINT_LEAST32_TYPE__		uint_least32_t;
#  define __DEFINED_UINT_LEAST32_T
# endif /* !__NEED_UINT_LEAST32_T */

# if defined(__NEED_UINT64_T) && !defined(__DEFINED_UINT64_T)
typedef __UINT64_TYPE__			uint_least64_t;
#  define __DEFINED_UINT64_T
# endif /* !__NEED_UINT64_T */

/* ===			end			=== */

#endif /* !_LIBC_BITS_TYPES_H_ */
