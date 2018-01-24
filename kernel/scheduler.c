/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/interrupts.h>
#include <kernel/scheduler.h>
#include <kernel/vaspace.h>
#include <stdio.h>

struct scheduler scheduler =
{
	.next = thread_table,
	.lock = SPINLOCK_DEFAULT,
};

static struct thread *
scheduler_get_next(void)
{
	struct thread *next;

	spinlock_acquire(&scheduler.lock);
	next = scheduler.next;
	spinlock_release(&scheduler.lock);
	return (next);

}

static void
scheduler_update_next(struct thread *next)
{
	spinlock_acquire(&scheduler.lock);
	scheduler.next = next;
	spinlock_release(&scheduler.lock);
}

/*
** Looks for the next runnable thread.
*/
static struct thread *
find_next_thread(void)
{
	bool pass;
	struct thread *next;
	struct thread const *limit;
	struct thread *t;

	pass = false;
	next = scheduler_get_next();
	t = next + 1;
	limit = thread_table + KCONFIG_MAX_THREADS;

look_for_next:
	while (t < limit)
	{
		spin_rwlock_acquire_read(&t->rwlock);
		if (t->state == RUNNABLE) {
			spin_rwlock_release_read(&t->rwlock);
			spin_rwlock_acquire_write(&t->rwlock);
			if (t->state == RUNNABLE) { /* Ensure state hasn't changed */
				scheduler_update_next(t);
				return (t);
			}
			spin_rwlock_release_write(&t->rwlock);
		} else {
			spin_rwlock_release_read(&t->rwlock);
		}
		++t;
	}
	if (!pass)
	{
		t = (struct thread *)thread_table;
		limit = next + 1;
		pass = true;
		goto look_for_next;
	}
	return (NULL);
}

/*
** Finds and switches to the next runnable thread.
** If no thread is available, halts and try again.
**
** Returns the stack of the new thread.
**
** This function is called by 'enter_scheduler', do not call it directly.
*/
void *
reschedule(void *old_sp)
{
	struct thread *new;
	struct thread *old;

	/* Save stack and release current thread. */
	old = current_thread();
	if (old) {
		old->stack_saved = old_sp;
		current_thread_release_write();
	}
	current_cpu()->thread = NULL;

	/* Find new thread or halt (to save energy) */
	new = find_next_thread();	/* new is already locked */
	while (!new) {
		enable_interrupts();	/* Halt the processor until next timer */
		halt();
		disable_interrupts();
		new = find_next_thread();
	}

	/* Switch to new thread */
	current_cpu()->thread = new;
	new->state = RUNNING;

	arch_set_kernel_stack((uintptr)new->kstack_top);
	arch_vaspace_switch(new->vaspace);

	current_thread_release_write();

#if KCONFIG_DEBUG_SCHEDULER
	printf("Running %s on core %zu\n", new->name, current_cpu_id());
#endif
	return (new->stack_saved);
}

/*
** Yields the cpu to an other thread.
**
** This function will return at a later time, or possibly immediately if no
** other threads are waiting to be executed (don't worry, they are guilty).
**
** _WARNING_:
** The current thread must NOT be acquired before calling this, NOR it's address space,
** or the kernel will enter an infinite loop.
*/
void yield()
{
	struct thread *t;
	int_state_t state;

	push_interrupts_state(&state);
	{
		disable_interrupts();

		/* Set current thread as runnable */
		t = current_thread_acquire_write(); /* release in scheduler() */
		assert_scheduler(t->state == RUNNING);
		t->state = RUNNABLE;

		enter_scheduler(current_cpu()->scheduler_stack_top);
	}
	pop_interrupts_state(&state);
}
