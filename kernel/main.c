/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/init.h>
#include <kernel/interrupts.h>
#include <kernel/memory.h>
#include <kernel/thread.h>
#include <kernel/syscall.h>
#include <arch/linker.h>
#include <stdio.h>

/*
** Kernel main entry point
*/
void
kmain(void)
{
	/* Put us in some sort of thread-context */
	thread_early_init();

	/* Trigger all init levels */
	trigger_init_levels(INIT_LEVEL_EARLIEST, INIT_LEVEL_LATEST);

	printf("Welcome to ChaOS\n\n");

	sys_exec("/init");
	panic("kmain's sys_exec() failed\n");
}

/* Mark the kernel as physically reserved */
NEW_PMM_RESERVED_AREA(kernel, NULL_FRAME, KERNEL_PHYSICAL_END);
