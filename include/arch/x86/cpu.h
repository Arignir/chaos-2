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

struct cpu_features
{

};

struct cpu
{
	struct cpu_features features;
	uchar vendor_id[13];
	struct mp_proc *conf;
};

/* Number of CPUs enabled. */
extern uint ncpu;

/* An array for all possible cpus. */
extern struct cpu cpus[KCONFIG_MAX_CORE];

#endif /* !_ARCH_X86_CPU_H_ */
