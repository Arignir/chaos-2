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
#include <kernel/initrd.h>
#include <kernel/exec.h>
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
** Returns a thread that has the NONE state, already locked as writer, or NULL
** if none were found.
*/
static struct thread *
find_free_thread(void)
{
	struct thread *t;

	t = thread_table;
	while (t < thread_table + KCONFIG_MAX_THREADS) {
		spin_rwlock_acquire_read(&t->rwlock);
		if (t->state == NONE) {
			spin_rwlock_release_read(&t->rwlock);
			spin_rwlock_acquire_write(&t->rwlock);
			if (t->state == NONE) { /* Ensure state hasn't changed */
				return (t);
			}
			spin_rwlock_release_write(&t->rwlock);
		} else {
			spin_rwlock_release_read(&t->rwlock);
		}
		++t;
	}
	return (NULL);
}

/*
** Creates both user and tkernel stacks for the given thread.
** The kernel stack is created only if t->kstack == NULL, otherwise it
** reuses the existing kernel stack.
**
** This assumes the given thread and it's virtual address space are both
** locked as writers.
*/
status_t
thread_create_stacks(struct thread *t)
{
	assert_thread(!t->stack);

	t->stack = vaspace_new_random_vseg(
		KCONFIG_THREAD_STACK_SIZE * PAGE_SIZE,
		MMAP_USER | MMAP_WRITE
	);
	if (!t->stack) {
		return (ERR_NO_MEMORY);
	}
	t->stack_top = (uchar *)t->stack + KCONFIG_THREAD_STACK_SIZE * PAGE_SIZE;
	if (!t->kstack) {
		t->kstack = kalloc(KCONFIG_KERNEL_STACK_SIZE * PAGE_SIZE);
		if (!t->kstack) {
			/* TODO FIXME Free thread's user stack here */
			return (ERR_NO_MEMORY);
		}
		t->kstack_top = (uchar *)t->kstack + KCONFIG_KERNEL_STACK_SIZE * PAGE_SIZE;
	}
	t->stack_saved = t->kstack_top;
	return (OK);
}

/*
** Creates a new thread starting at the given function.
**
** The thread will have the current virtual address space attached, and a stack
** will be allocated.
**
** Current thread and virtual address space must /NOT/ be locked as writer.
*/
status_t
thread_clone(void *ip)
{
	status_t s;
	struct thread *t;
	struct vaspace *vaspace;

	t = find_free_thread();
	if (!t)
		return (ERR_NO_MORE_ID);

	current_thread_acquire_write();
	vaspace = current_vaspace_acquire_write();

	thread_set_name(t, current_thread()->name);
	if ((s = thread_create_stacks(t))) {
		goto err;
	}

	t->exit_status = 0;
	t->state = RUNNABLE;
	t->parent = current_thread();
	t->entry = ip;
	t->user = t->parent->user;
	t->cwd = strdup(t->parent->cwd);

	thread_attach_vaspace(t, vaspace);

	/*
	** Sets the arch-dependant part of cloning a thread.
	** This mainly consist on setting up a proper stack so the scheduler
	** won't be lost when switching to the new thread.
	*/
	arch_thread_clone(t);

	current_vaspace_release_write();
	current_thread_release_write();
	spin_rwlock_release_write(&t->rwlock);
	return (OK);
err:
	current_vaspace_release_write();
	current_thread_release_write();
	spin_rwlock_release_write(&t->rwlock);
	return (s);
}

/*
** Exits the current thread with the given exit status.
**
** If the current thread is the last one holding the current vaspace, then
** the memory is freed.
**
** This places the given thread in a ZOMBIE state, where most memory
** is freed but there is still a little bit that needs to be freed from
** a different virtual address space (kernel stack and some arch-dependant stuff).
** This is done when an other thread waits for this one.
**
** Current thread and current virtual address space must be locked.
*/
__noreturn
void
thread_exit(uchar status)
{
	struct thread *t;

	t = current_cpu()->thread;

	if (unlikely(t == thread_table))
		panic("init got killed.");

	/*
	** Free user stack.
	** We can't free kernel stack from here because we are still in it.
	*/
	vaspace_remove_vseg(t->stack, MUNMAP_DEFAULT);

	/* Detach (and eventually free) virtual address space */
	thread_detach_vaspace();
	t->exit_status = status;
	zombifie();
	panic("Leaving thread_exit()\n");
}

/*
** Executes the given binary, detaching iteself from the current virtual address
** space to create a new one.
**
** Current thread and current virtual address space must be locked.
*/
status_t
thread_exec(char const *path __unused /* TODO */)
{
	status_t s;
	struct initrd_virt const *virt = initrd_get_virtual();

	if ((s = thread_detach_and_create_vaspace())) {
		return (s);
	}

	/* exec will release the virtual address space and the current thread */
	exec(virt->start, virt->len);

	/* If exec failed, we have no other choice but kill the process */
	thread_exit(1);
}

/*
** Attaches the given virtual space to the given thread.
** This assumes there is no previous virtual space already attached.
**
** This assumes that the given thread 'thread' and the given
** virtual memory space 'vaspace' are locked as writers.
*/
void
thread_attach_vaspace(struct thread *thread, struct vaspace *vaspace)
{
	thread->vaspace = vaspace;
	++vaspace->count;
}

/*
** Detaches the current thread from it's virtual space.
** The current thread's virtual address space is set to NULL if the vaspace is released,
** or left untouched otherwise.
**
** This assumes that the current thread and it's virtual address space
** are both locked as writers.
*/
void
thread_detach_vaspace(void)
{
	struct thread *t;
	struct vaspace *vaspace;

	t = current_thread();
	vaspace = current_vaspace();
	--vaspace->count;
	if (!vaspace->count) {
		vaspace_free(); /* Switches to a neutral vaspace */
		t->vaspace = NULL;
	} else {
		current_vaspace_release_write();
	}
}

/*
** Detaches the current thread from it's virtual address space, and attaches it to a new
** one.
**
** This assumes that the current thread and it's virtual address space
** are both locked as writers.
**
** In case of error, the current virtual address space is kept untouched.
*/
status_t
thread_detach_and_create_vaspace(void)
{
	struct thread *t;
	struct vaspace *vaspace;
	struct vaspace *new_vaspace;

	t = current_thread();
	vaspace = current_vaspace();
	if (vaspace->count == 1) {
		vaspace_cleanup();
		vaspace_add_kernel_vseg(vaspace);
	} else {
		new_vaspace = arch_new_vaspace();
		if (!new_vaspace) {
			return (ERR_NO_MEMORY);
		}
		arch_vaspace_switch(new_vaspace);

		spin_rwlock_acquire_write(&new_vaspace->rwlock);
		thread_attach_vaspace(t, new_vaspace);

		--vaspace->count;
		spin_rwlock_release_write(&vaspace->rwlock);
	}
	t->stack = NULL;
	t->stack_top = NULL;
	t->stack_saved = NULL;
	return (OK);
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
		spin_rwlock_init(&thread_table[i].rwlock);
		thread_table[i].state = NONE;
		++i;
	}

	current_cpu()->thread = thread_table;

	/* Init boot thread */
	t = current_thread_acquire_write();
	{
		thread_set_name(t, "boot");
		t->state = RUNNING;
	}
	current_thread_release_write();
}

/*
** Sets up the kthread thread
*/
void
thread_init(void)
{
	struct thread *t;

	t = current_thread_acquire_write();
	{
		thread_set_name(t, "kthread");
		t->cwd = strdup("/");

		/* Setup vaspace */
		assert_eq(vaspace_init(&kthread_vaspace), OK);
		spin_rwlock_acquire_write(&kthread_vaspace.rwlock);
		{
			thread_attach_vaspace(t, &kthread_vaspace);
		}
		spin_rwlock_release_write(&kthread_vaspace.rwlock);
	}
	current_thread_release_write();
}
