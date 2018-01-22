/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_THREAD_H_
# define _KERNEL_THREAD_H_

# include <kconfig.h>
# include <chaosdef.h>
# include <chaoserr.h>
# include <kernel/memory.h>
# include <kernel/spinlock.h>

typedef int tid_t;
typedef int (*thread_main)();

# if KCONFIG_DEBUG_THREAD
#  define assert_thread(x) assert(x)
# else
#  define assert_thread(x)
# endif /* KCONFIG_DEBUG_THREAD */

/*
** An enum of all states a thread can be in.
*/
enum			thread_state
{
	NONE = 0,
	SUSPENDED,
	RUNNABLE,
	RUNNING,
	BLOCKED,
	SLEEPING,
	ZOMBIE,
};

/*
** Strings to print each thread state
*/
static char const *thread_state_str[] =
{
	[NONE]		= "NONE",
	[SUSPENDED]	= "SUSPENDED",
	[RUNNABLE]	= "RUNNABLE",
	[RUNNING]	= "RUNNING",
	[BLOCKED]	= "BLOCKED",
	[SLEEPING]	= "SLEEPING",
	[ZOMBIE]	= "ZOMBIE",
};

struct			thread
{
	/* Thread basic infos*/
	char name[256];			/* Name */
	uchar exit_status;		/* Exit status */
	enum thread_state state;	/* Curent thread state */
	struct thread *parent;		/* Parent thread */

	/* Thread stack */
	virtaddr_t stack;	/* Bottom of user stack */
	virtaddr_t stack_top;	/* Top of user stack. */
	virtaddr_t stack_saved;	/* Stack saved when a context switch occures */

	/* Kernel stack */
	virtaddr_t kstack;	/* Bottom of kernel stack */
	virtaddr_t kstack_top;	/* Top of user stack */

	/* entry point */
	thread_main entry;

	/* virtual address space */
	struct vaspace *vaspace;

	/* Locker of this thread. Ideally, this should be a spinlock based rwlock */
	struct spinlock lock;
};

extern struct thread thread_table[];

void			thread_early_init(void);
void			thread_init(void);
status_t		thread_create_stacks(struct thread *t);
status_t		thread_clone(void *ip);
void			thread_attach_vaspace(struct thread *t, struct vaspace *vaspace);

/* Arch-dependant function, usually in assembly */
void			arch_jump_to_userspace(void *stack, void (*main)(void));
void			arch_set_kernel_stack(uintptr);
void			arch_thread_clone(struct thread *t);

#endif /* !_KERNEL_THREAD_H_ */
