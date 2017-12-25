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
# include <arch/atomic.h>

/*
** A structure representing a simple spin lock
*/
struct spinlock
{
	uint lock;		/* Is the lock held ? */
};

# define SPINLOCK_DEFAULT			\
	{					\
		.lock = 0,			\
	}

/*
** Inits a spinlock
*/
static inline void
spinlock_init(struct spinlock *sl)
{
	*sl = (struct spinlock)SPINLOCK_DEFAULT;
}

/*
** Acquires a spinlock
*/
static inline void
spinlock_acquire(struct spinlock *sl)
{
	while (atomic_exchange(&sl->lock, 1) != 0);
}

/*
** Releases a spinlocks
*/
static inline void
spinlock_release(struct spinlock *sl)
{
	atomic_exchange(&sl->lock, 0);
}

#endif /* !_KERNEL_SPINLOCK_H_ */
