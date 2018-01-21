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
# include <kernel/thread.h>
# include <arch/cpu.h>

/* A structure representing a CPU */
struct cpu
{
	struct arch_cpu;	/* Arch dependant stuff */
	struct thread *thread;	/* Currently running thread */

	bool bsp;		/* Is it the bootstrap processor ? */
	uint started;		/* Is the cpu started ? */

	void *scheduler_stack;	/* Stack for the scheduler. Also the boot stack. */
	void *scheduler_stack_top;

	uint int_count;		/* Number of recursion of cpu_push_ints() */
	int_state_t int_state;	/* State of the ints at the first call of cpu_push_ints */

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
size_t		current_cpu_id(void);

/*
** Locks the currently running thread and returns a constant pointer to it.
**
** Remember to release the thread later.
*/
static inline struct thread *
current_thread_acquire(void)
{
	struct thread *t;

	t = current_cpu()->thread;
	spinlock_acquire(&t->lock);
	return (t);
}

/*
** Release the currently running thread.
*/
static inline void
current_thread_release(void)
{
	struct thread *t;

	t = current_cpu()->thread;
	spinlock_release(&t->lock);
}

#endif /* !_KERNEL_CPU_H_ */
