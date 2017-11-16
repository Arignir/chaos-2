/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_CPU_H_
# define _KERNEL_CPU_H_

# include <chaosdef.h>
# include <kconfig.h>

/*
** This file relies on a common implementation of the following items for each
** architectures.
*/

/* A structure representing a CPU */
struct cpu;

/* Number of CPUs on the current system. */
extern uint ncpu;

/* An array for all possible cpus. */
extern struct cpu cpus[KCONFIG_MAX_CORE];

struct cpu	*current_cpu(void);

#endif /* !_KERNEL_CPU_H_ */
