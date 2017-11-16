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

/*
** An enum to associate a label for each MSRs.
*/
enum msr_id
{
	IA32_APIC_BASE		= 0x1B,
};

/*
** For a description of the following functions,
** please refer to Intel's Instruction Set Reference.
*/

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
cpuid(uint32 code, uint32 *eax, uint32 *edx)
{
	asm volatile("cpuid" : "=a"(*eax), "=d"(*edx) : "a"(code) : "ebx", "ecx");
}

static inline void
cpuid_string(uint32 code, uchar str[12])
{
	asm volatile("cpuid" : "=b"(*(uint32 *)str), "=d"(*(uint32 *)(str + 4)), "=c"(*(uint32 *)(str + 8)) : "a"(code));
}

static inline uint64
read_msr(enum msr_id msr)
{
	uint32 low;
	uint32 high;

	asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"((uint32)msr));
	return (((uint64)high << 32u) | (uint64)low);
}

static inline void
write_msr(enum msr_id msr, uint64 val)
{
	uint32 low;
	uint32 high;

	low = (uint32)val;
	high = (uint32)(val >> 32u);
	asm volatile("wrmsr" : : "a"(low), "d"(high), "c"((uint32)msr));
}

#endif /* !_ARCH_X86_ASM_H_ */
