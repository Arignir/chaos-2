/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_MUTEX_H_
# define _KERNEL_MUTEX_H_

# include <kernel/semaphore.h>

struct mutex
{
	struct semaphore;
};

# define MUTEX_DEFAULT SEMAPHORE_DEFAULT(1)

static inline void
mutex_init(struct mutex *mutex)
{
	semaphore_init((struct semaphore *)mutex, 1);
}

static inline void
mutex_acquire(struct mutex *mutex)
{
	semaphore_acquire((struct semaphore *)mutex);
}

static inline void
mutex_release(struct mutex *mutex)
{
	semaphore_release((struct semaphore *)mutex);
}

static inline void
mutex_try_acquire(struct mutex *mutex)
{
	semaphore_try_acquire((struct semaphore *)mutex);
}

#endif /* !_KERNEL_MUTEX_H_ */
