/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_SPIN_RWLOCK_H_
# define _KERNEL_SPIN_RWLOCK_H_

# include <chaosdef.h>
# include <kernel/spinlock.h>

/*
** A structure representing a read-write lock
*/
struct spin_rwlock
{
	uint32 volatile readcount;
	struct spinlock resource_access;
	struct spinlock readcount_access;
	struct spinlock service_queue;
};

# define SPIN_RWLOCK_DEFAULT				\
	{						\
		.readcount = 0,				\
		.resource_access = SPINLOCK_DEFAULT,	\
		.readcount_access = SPINLOCK_DEFAULT,	\
		.service_queue = SPINLOCK_DEFAULT,	\
	}

static inline void
spin_rwlock_init(struct spin_rwlock *spin_rwlock)
{
	*spin_rwlock = (struct spin_rwlock)SPIN_RWLOCK_DEFAULT;
}

static inline void
spin_rwlock_acquire_read(struct spin_rwlock *rw)
{
	spinlock_acquire(&rw->service_queue);
	spinlock_acquire(&rw->readcount_access);

	if (!rw->readcount) {
		spinlock_acquire(&rw->resource_access);
	}
	++rw->readcount;

	spinlock_release(&rw->service_queue);
	spinlock_release(&rw->readcount_access);
}

static inline void
spin_rwlock_release_read(struct spin_rwlock *rw)
{
	--rw->readcount;
	if (!rw->readcount) {
		spinlock_release(&rw->resource_access);
	}
	spinlock_release(&rw->readcount_access);
}

static inline void
spin_rwlock_acquire_write(struct spin_rwlock *rw)
{
	spinlock_acquire(&rw->service_queue);
	spinlock_acquire(&rw->resource_access);
	spinlock_release(&rw->service_queue);
}

static inline void
spin_rwlock_release_write(struct spin_rwlock *rw)
{
	spinlock_release(&rw->resource_access);
}

#endif /* !_KERNEL_SPIN_RWLOCK_H_ */
