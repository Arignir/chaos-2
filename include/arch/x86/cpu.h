/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_CPU_H_
# define _ARCH_X86_CPU_H_

# include <chaosdef.h>

/*
** CPUID features bitfield
*/
enum cpuid_features {
	/* EDX */
	CPUID_EDX_X87		= 1 << 0, /* x87 FPU on chip */
	CPUID_EDX_MSR		= 1 << 5, /* RDMSR/WRMSR */
	CPUID_EDX_APIC		= 1 << 9, /* Local APIC on chip */
	CPUID_EDX_SSE		= 1 << 25, /* Streaming SIMD extensions (SSE) */
	CPUID_EDX_SSE2		= 1 << 26, /* Streaming SIMD extensions 2 (SSE2) */
};

/*
** A structure representing a single CPU
*/
struct cpu
{
	uchar vendor_id[13];
	uint32 signature; /* eax value of cpuid.01h */
	uint32 features; /* edx value of cpuid.01h - see 'enum cpuid_features' */
	uint32 lapic_id; /* Local APIC id */
	uint started;
};

void	detect_cpu_features(void);

#if KCONFIG_ENABLE_SMP
void	ap_setup(void);
#endif /* KCONFIG_ENABLE_SMP */

# include <kernel/cpu.h>

#endif /* !_ARCH_X86_CPU_H_ */
