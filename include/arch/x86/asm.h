/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_ASM_H_
# define _ARCH_X86_ASM_H_

# include <chaosdef.h>

static inline void
outb(ushort port, uchar data)
{
	asm volatile("out %0, %1" :: "a"(data), "d"(port));
}

static inline uchar
inb(ushort port)
{
	uchar data;

	asm volatile("in %1, %0" : "=a"(data) : "d"(port));
	return (data);
}

static inline void
cpuid(int code, uint32_t *eax, uint32_t *edx)
{
	asm volatile("cpuid" : "=a"(*eax), "=d"(*edx) : "a"(code) : "ecx", "ebx");
}

static inline void
cpuid_string(int code, uchar str[12])
{
	asm volatile("cpuid" : "=b"(*(uint32 *)str), "=d"(*(uint32 *)(str + 4)), "=c"(*(uint32 *)(str + 8)) : "a"(code));
}

#endif /* !_ARCH_X86_ASM_H_ */
