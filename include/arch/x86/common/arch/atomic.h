/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_COMMON_ARCH_ATOMIC_H_
# define _ARCH_X86_COMMON_ARCH_ATOMIC_H_

# include <arch/x86/asm.h>

static inline uint
atomic_exchange(volatile uint *ptr, uint value)
{
	return (xchg(ptr, value));
}

#endif /* !_ARCH_X86_COMMON_ARCH_ATOMIC_H_ */
