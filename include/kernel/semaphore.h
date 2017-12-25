/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_SEMAPHORE_H_
# define _KERNEL_SEMAPHORE_H_

# include <chaoserr.h>
# include <kernel/scheduler.h>
# include <kernel/spinlock.h>
# include <arch/atomic.h>

struct semaphore
{
	int volatile count;
	struct spinlock lock;
};

# define SEMAPHORE_DEFAULT(c)					\
	{							\
		.count = c,					\
		.lock = SPINLOCK_DEFAULT,			\
	}

/*
** Inits a semaphore with the given counter
*/
static inline void
semaphore_init(struct semaphore *sem, uint count)
{
	*sem = (struct semaphore)SEMAPHORE_DEFAULT(count);
}

/*
** Acquires the semaphore, or waits until it's possible.
*/
static inline void
semaphore_acquire(struct semaphore *sem)
{
	spinlock_acquire(&sem->lock);

	while (sem->count <= 0) {
		spinlock_release(&sem->lock);
		reschedule();
		spinlock_acquire(&sem->lock);
	}

	--sem->count;
	spinlock_release(&sem->lock);
}

/*
** Releases a semaphore, by incrmenting it's counter by one.
*/
static inline void
semaphore_release(struct semaphore *sem)
{
	spinlock_acquire(&sem->lock);

	++sem->count;

	spinlock_release(&sem->lock);
}

/*
**
*/
static inline status_t
semaphore_try_acquire(struct semaphore *sem)
{
	status_t s = OK;

	spinlock_acquire(&sem->lock);

	if (sem->count <= 0) {
		s = ERR_BAD_STATE;
	} else {
		--sem->count;
	}

	spinlock_release(&sem->lock);
	return (s);
}

#endif /* !_KERNEL_SEMAPHORE_H_ */
