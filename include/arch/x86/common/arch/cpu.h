/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_COMMON_ARCH_CPU_H_
# define _ARCH_X86_COMMON_ARCH_CPU_H_

# include <chaosdef.h>

/*
** A structure representing a single CPU
*/
struct arch_cpu
{
	uchar vendor_id[13];
	uint32 signature; /* eax value of cpuid.01h */
	uint32 features; /* edx value of cpuid.01h - see 'enum cpuid_features' */
	uint32 lapic_id; /* Local APIC id */
	uint started;
};

#endif /* !_ARCH_X86_COMMON_ARCH_CPU_H_ */
