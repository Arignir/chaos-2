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
#include <string.h>

/* Thread table */
struct thread thread_table[KCONFIG_MAX_THREADS] = { 0 };

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
** Put us in a thread context
** Called directly from kmain()
*/
void
thread_early_init(void)
{
	struct thread *t;

	t = thread_table;
	thread_set_name(t, "boot");
	t->tid = 0;
	t->state = RUNNING;
}
