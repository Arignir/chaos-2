/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_RWLOCK_H_
# define _KERNEL_RWLOCK_H_

# include <chaosdef.h>
# include <kernel/mutex.h>

/*
** A structure representing a read-write lock
*/
struct rwlock
{
	uint32 volatile readcount;
	struct mutex resource_access;
	struct mutex readcount_access;
	struct mutex service_queue;
};

# define RWLOCK_DEFAULT					\
	{						\
		.readcount = 0,				\
		.resource_access = MUTEX_DEFAULT,	\
		.readcount_access = MUTEX_DEFAULT,	\
		.service_queue = MUTEX_DEFAULT,		\
	}

static inline void
rwlock_init(struct rwlock *rwlock)
{
	*rwlock = (struct rwlock)RWLOCK_DEFAULT;
}

static inline void
rwlock_acquire_read(struct rwlock *rw)
{
	mutex_acquire(&rw->service_queue);
	mutex_acquire(&rw->readcount_access);

	if (!rw->readcount) {
		mutex_acquire(&rw->resource_access);
	}
	++rw->readcount;

	mutex_release(&rw->service_queue);
	mutex_release(&rw->readcount_access);
}

static inline void
rwlock_release_read(struct rwlock *rw)
{
	--rw->readcount;
	if (!rw->readcount) {
		mutex_release(&rw->resource_access);
	}
	mutex_release(&rw->readcount_access);
}

static inline void
rwlock_acquire_write(struct rwlock *rw)
{
	mutex_acquire(&rw->service_queue);
	mutex_acquire(&rw->resource_access);
	mutex_release(&rw->service_queue);
}

static inline void
rwlock_release_write(struct rwlock *rw)
{
	mutex_release(&rw->resource_access);
}

#endif /* !_KERNEL_RWLOCK_H_ */
