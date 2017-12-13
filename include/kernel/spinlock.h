/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_SPINLOCK_H_
# define _KERNEL_SPINLOCK_H_

# include <chaosdef.h>
# include <kernel/cpu.h>
# include <arch/atomic.h>

/*
** A structure representing a simple spin lock
*/
struct spinlock
{
	uint lock;		/* Is the lock held ? */
	struct cpu *holder;	/* The cpu holding the lock */
};

# define SPINLOCK_DEFAULT			\
	(struct spinlock) {			\
		.lock = 0,			\
		.holder = NULL,			\
	}

static inline void
spinlock_init(struct spinlock *sl)
{
	*sl = SPINLOCK_DEFAULT;
}

static inline void
spinlock_acquire(struct spinlock *sl)
{
	cpu_push_ints();
	while (atomic_exchange(&sl->lock, 1) != 0);

	/* Tells the C Compiler to issue a full memory barrier */
	memory_barrier();

	sl->holder = current_cpu();
}

static inline void
spinlock_release(struct spinlock *sl)
{
	sl->holder = NULL;
	memory_barrier();
	atomic_exchange(&sl->lock, 0);
	cpu_pop_ints();
}

#endif /* !_KERNEL_SPINLOCK_H_ */
