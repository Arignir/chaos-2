/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _CHAOSDEF_H_
# define _CHAOSDEF_H_

# include <stddef.h>
# include <stdint.h>
# include <stdbool.h>

/*
** A useful set of macros that act like keywords that are not available otherwise in C11.
*/
# define inline			__inline
# define asm			__asm__
# define restrict		__restrict
# define __pure			__attribute__((pure))
# define __const		__attribute__((const))
# define __cold			__attribute__((cold))
# define __hot			__attribute__((hot))
# define __used			__attribute__((used))
# define __unused		__attribute__((unused))
# define __packed		__attribute__((packed))
# define __weak			__attribute__((weak))
# define __aligned(x)		__attribute__((aligned (x)))
# define __section(s)		__attribute__((section (s)))
# define __noreturn		__attribute__((noreturn))
# define likely(x)		__builtin_expect((x), 1)
# define unlikely(x)		__builtin_expect((x), 0)
# define __optimize(x)		__attribute__((optimize(x)))

/* Defines some shortcuts types. */
typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;
typedef unsigned long long	ulonglong;
typedef intmax_t		intmax;
typedef uintmax_t		uintmax;

/* Defines some shortcuts for size-dependant types. */
typedef int8_t			int8;
typedef int16_t			int16;
typedef int32_t			int32;
typedef int64_t			int64;
typedef intptr_t		intptr;
typedef intptr_t		ssize_t;

typedef uint8_t			uint8;
typedef uint16_t		uint16;
typedef uint32_t		uint32;
typedef uint64_t		uint64;
typedef uintptr_t		uintptr;

/* ChaOS Object's Handler */
typedef uint			handler_t;
typedef handler_t		file_handler_t;

/* Print a message and halt the computer. */
void				panic(char const *fmt, ...) __noreturn;

# define static_assert(e)	extern char (*__static_assert(void)) [sizeof(char[1 - 2 * !(e)])]

/* Panics if the given expression is evaluated to false */
# define			assert(expr)					\
	do {									\
		if (unlikely(!(expr))) {					\
			panic("assert(%s) failed (in %s at line %u).\n",	\
				#expr, __FILE__, __LINE__);			\
		}								\
	}									\
	while (0)

/* Panics if the given operands ARE NOT equal. */
# define assert_eq(a, b)	assert((a) == (b))

/* Panics if the given operands ARE equal. */
# define assert_ne(a, b)	assert((a) != (b))

/* Panics if the first operand is lower than the other one. */
# define assert_lg(a, b)	assert((a) < (b))

/* Panics if the first operand is lower or equal to the other one. */
# define assert_le(a, b)	assert((a) <= (b))

/* Panics if the first operand is greater than the other one. */
# define assert_gt(a, b)	assert((a) > (b))

/* Panics if the first operand is greater or equal to the other one. */
# define assert_ge(a, b)	assert((a) > (b))

/* Main entry point of the kernel. Called by assembly bootstrap code */
void	kmain(void);

/* Mask to test if an address is page-aligned */
# define PAGE_SIZE_MASK		((PAGE_SIZE) - 1u)

/* Tests if a given address is a multiple of PAGE_SIZE */
# define IS_PAGE_ALIGNED(x)	(!((uintptr)(x) & PAGE_SIZE_MASK))

/* Round down the given number to the given power of 2 */
# define ROUND_DOWN(x, y)	((x) & ~((y) - 1))

/* Align the given number to the greatest-or-equal given power of 2 */
# define ALIGN(x, y)		(((x) + ((y) - 1)) & ~((y) - 1))

/* Align the given number to PAGE_SIZE */
# define PAGE_ALIGN(x)		(ALIGN(x, PAGE_SIZE))

#endif /* !_CHAOSDEF_H_ */
