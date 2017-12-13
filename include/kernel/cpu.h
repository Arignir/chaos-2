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
# include <kernel/interrupts.h>
# include <arch/cpu.h>

/* A structure representing a CPU */
struct cpu
{
	struct arch_cpu;	/* Arch dependant stuff */
	struct thread *thread;	/* Currently running thread */
	uint int_count;		/* Number of recursion of cpu_push_ints() */
	int_state_t int_state;	/* State of the ints at the first call of cpu_push_ints */
	bool bsp;		/* Is it the bootstrap processor ? */
};

/* Number of CPUs on the current system. */
extern uint ncpu;

/* An array for all possible cpus. */
extern struct cpu cpus[KCONFIG_MAX_CPUS];

/* The bootstrap processor. Used only at boot time. */
extern struct cpu *bsp;

/* Tells if the BSP has been remaped to it's corresponding entry within the cpu table. */
extern bool bsp_remapped;

/* Arch-dependant function to return the current cpu */
struct cpu	*current_cpu(void);

void		cpu_push_ints(void);
void		cpu_pop_ints(void);
void		cpu_remap_bsp(void);

#endif /* !_KERNEL_CPU_H_ */
