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
** Attaches the given virtual space to the given thread.
** This assumes there is no previous virtual space already attached.
**
** This function assumes that the given thread 'thread' and the given
** virtual memory space 'vaspace' and locked as writers (or there is no
** need to do so).
*/
static void
thread_attach_vaspace(struct thread *t, struct vaspace *vaspace)
{
	assert_thread(!t->vaspace);

	t->vaspace = vaspace;
	++t->vaspace->count;
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

	/* Init all locks */
	i = 0;
	while (i < KCONFIG_MAX_THREADS) {
		rwlock_init(&thread_table[i].rwlock);
		++i;
	}

	/* Init boot thread */
	t = thread_table;
	rwlock_acquire_write(&t->rwlock);
	{
		thread_set_name(t, "boot");
		t->tid = 0;
		t->state = EMBRYO;
	}
	rwlock_release_write(&t->rwlock);
}

/*
** Sets up the kthread thread
*/
void
thread_init(void)
{
	struct thread *t;

	t = thread_table;
	rwlock_acquire_write(&t->rwlock);
	{
		thread_set_name(t, "kthread");

		/* Setup vaspace */
		vaspace_init(&kthread_vaspace);
		rwlock_acquire_write(&kthread_vaspace.rwlock);
		{
			thread_attach_vaspace(t, &kthread_vaspace);
		}
		rwlock_release_write(&kthread_vaspace.rwlock);

		t->state = RUNNABLE;
	}
	rwlock_release_write(&t->rwlock);
}
