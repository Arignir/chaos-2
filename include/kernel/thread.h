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
# include <kernel/spin_rwlock.h>
# include <kernel/cpu.h>

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
	RUNNABLE,
	RUNNING,
	ZOMBIE,
};

/*
** Strings to print each thread state
*/
static char const *thread_state_str[] =
{
	[NONE]		= "NONE",
	[RUNNABLE]	= "RUNNABLE",
	[RUNNING]	= "RUNNING",
	[ZOMBIE]	= "ZOMBIE",
};

struct			thread
{
	/* Thread basic infos*/
	char name[256];			/* Name */
	uchar exit_status;		/* Exit status */
	enum thread_state state;	/* Curent thread state */
	struct thread *parent;		/* Parent thread */
	bool user;			/* True for user, false for kernel threads */

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
	struct spin_rwlock rwlock;
};

extern struct thread thread_table[];

void			thread_early_init(void);
void			thread_init(void);
__noreturn void		thread_exit(uchar status);
status_t		thread_exec(char const *path);
status_t		thread_create_stacks(struct thread *t);
status_t		thread_clone(void *ip);
void			thread_attach_vaspace(struct thread *t, struct vaspace *vaspace);
void			thread_detach_vaspace(void);
status_t		thread_detach_and_create_vaspace(void);

/* Arch-dependant function, usually in assembly */
void			arch_jump_to_userspace(void *stack, void (*main)(void));
void			arch_set_kernel_stack(uintptr);
void			arch_thread_clone(struct thread *t);

static inline struct thread *
current_thread(void)
{
	return (current_cpu()->thread);
}

static inline struct thread *
current_thread_acquire_read(void)
{
	struct thread *t;

	t = current_thread();
	spin_rwlock_acquire_read(&t->rwlock);
	return (t);
}

static inline struct thread *
current_thread_acquire_write(void)
{
	struct thread *t;

	t = current_thread();
	spin_rwlock_acquire_write(&t->rwlock);
	return (t);
}

static inline void
current_thread_release_read(void)
{
	struct thread *t;

	t = current_thread();
	spin_rwlock_release_read(&t->rwlock);
}

static inline void
current_thread_release_write(void)
{
	struct thread *t;

	t = current_thread();
	spin_rwlock_release_write(&t->rwlock);
}

#endif /* !_KERNEL_THREAD_H_ */
