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
# include <kconfig.h>
# include <arch/x86/smp.h>

enum cpuid_features {
	/* EDX */
	CPUID_EDX_X87		= 1 << 0, /* x87 FPU on chip */
	CPUID_EDX_APIC		= 1 << 9, /* Local APIC on chip */
	CPUID_EDX_SSE		= 1 << 25, /* Streaming SIMD extensions (SSE) */
	CPUID_EDX_SSE2		= 1 << 26, /* Streaming SIMD extensions 2 (SSE2) */
};

struct cpu
{
	uchar vendor_id[13];
	uint32 signature; /* eax value of cpuid.01h */
	uint32 features; /* edx value of cpuid.01h - see 'enum cpuid_features' */
	struct mp_proc *conf;
};

/* Number of CPUs enabled. */
extern uint ncpu;

/* An array for all possible cpus. */
extern struct cpu cpus[KCONFIG_MAX_CORE];

#endif /* !_ARCH_X86_CPU_H_ */
