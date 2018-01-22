/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kconfig.h>
#include <kernel/thread.h>
#include <kernel/vaspace.h>
#include <kernel/rwlock.h>
#include <kernel/kalloc.h>
#include <string.h>

/* Thread table */
struct thread thread_table[KCONFIG_MAX_THREADS] = { 0 };

/* The virtual address space for the kthread process. */
static struct vaspace kthread_vaspace;

/*
** Sets the name of the given thread.
*/
static void
thread_set_name(struct thread *t, char const *name)
{
	strncpy(t->name, name, sizeof(t->name) - 1);
	t->name[sizeof(t->name) - 1] = '\0';
}

/*
** Returns a thread that has the NONE state, already locked, or NULL
** if none were found.
*/
static struct thread *
find_free_thread(void)
{
	struct thread *t;

	t = thread_table;
	while (t < thread_table + KCONFIG_MAX_THREADS) {
		spinlock_acquire(&t->lock);
		if (t->state == NONE) {
			return (t);
		}
		spinlock_release(&t->lock);
		++t;
	}
	return (NULL);
}

/*
** Creates both user and kernel stacks for the given thread.
**
** This assumes the given thread and it's virtual address space are both
** locked as writers.
*/
status_t
thread_create_stacks(struct thread *t)
{
	assert_thread(!t->stack);
	assert_thread(!t->kstack);

	t->stack = vaspace_new_random_vseg(
		KCONFIG_THREAD_STACK_SIZE * PAGE_SIZE,
		MMAP_USER | MMAP_WRITE
	);
	if (!t->stack) {
		return (ERR_NO_MEMORY);
	}
	t->kstack = kalloc(KCONFIG_KERNEL_STACK_SIZE * PAGE_SIZE);
	if (!t->kstack) {
		/* TODO FIXME Free thread stack here */
		return (ERR_NO_MEMORY);
	}
	t->stack_top = (uchar *)t->stack + KCONFIG_THREAD_STACK_SIZE * PAGE_SIZE;
	t->kstack_top = (uchar *)t->kstack + KCONFIG_KERNEL_STACK_SIZE * PAGE_SIZE;
	t->stack_saved = t->kstack_top;
	return (OK);
}

/*
** Creates a new thread starting at the given function.
**
** The thread will have the current virtual address space attached, and a stack
** will be allocated.
**
** Current virtual address space MUST be locked as writer.
*/
status_t
thread_clone(void *ip)
{
	status_t s;
	struct thread *t;
	struct vaspace *vaspace;

	vaspace = current_vaspace();
	t = find_free_thread();
	if (!t)
		return (ERR_NO_MORE_ID);

	thread_set_name(t, current_cpu()->thread->name);
	if ((s = thread_create_stacks(t))) {
		goto err;
	}

	t->exit_status = 0;
	t->state = RUNNABLE;
	t->parent = current_cpu()->thread;
	t->entry = ip;
	thread_attach_vaspace(t, vaspace);

	/*
	** Sets the arch-dependant part of cloning a thread.
	** This mainly consist on setting up a proper stack so the scheduler
	** won't be lost when switching to the new thread.
	*/
	arch_thread_clone(t);

	spinlock_release(&t->lock);
	return (OK);
err:
	spinlock_release(&t->lock);
	return (s);
}

/*
** Attaches the given virtual space to the given thread.
** This assumes there is no previous virtual space already attached.
**
** This assumes that the given thread 'thread' and the given
** virtual memory space 'vaspace' are locked as writers (or there is no
** need to do so).
*/
void
thread_attach_vaspace(struct thread *thread, struct vaspace *vaspace)
{
	thread->vaspace = vaspace;
	++vaspace->count;
}

/*
** Put us in a thread context
** Called directly from kmain()
*/
void
thread_early_init(void)
{
	struct thread *t;
	size_t i;

	/* Init thread table */
	i = 0;
	while (i < KCONFIG_MAX_THREADS) {
		spinlock_init(&thread_table[i].lock);
		thread_table[i].state = NONE;
		++i;
	}

	/* Init boot thread */
	t = thread_table;
	spinlock_acquire(&t->lock);
	{
		thread_set_name(t, "boot");
		t->state = RUNNING;
	}
	spinlock_release(&t->lock);
}

/*
** Sets up the kthread thread
*/
void
thread_init(void)
{
	struct thread *t;

	t = thread_table;
	spinlock_acquire(&t->lock);
	{
		thread_set_name(t, "kthread");

		/* Setup vaspace */
		assert_eq(vaspace_init(&kthread_vaspace), OK);
		rwlock_acquire_write(&kthread_vaspace.rwlock);
		{
			thread_attach_vaspace(t, &kthread_vaspace);
		}
		rwlock_release_write(&kthread_vaspace.rwlock);
	}
	spinlock_release(&t->lock);
}
