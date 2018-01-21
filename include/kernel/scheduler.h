/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_SCHEDULER_H_
# define _KERNEL_SCHEDULER_H_

# include <kconfig.h>
# include <chaosdef.h>
# include <kernel/spinlock.h>

struct scheduler
{
	struct thread *next;
	struct spinlock lock;
};

struct thread;

# if DEBUG_SCHEDULER
#  define assert_scheduler(x) assert(x)
# else
#  define assert_scheduler(x)
# endif /* DEBUG_SCHEDULER */

/*
** Arch-dependant functions that must be reimplemented for each architecture.
*/
extern void	enter_scheduler(void *scheduler_stack);

void		*reschedule(void *);
void		yield(void);

#endif /* !_KERNEL_SCHEDULER_H_ */
