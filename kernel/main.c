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
#include <kernel/linker.h>
#include <kernel/memory.h>
#include <stdio.h>

/*
** Kernel main entry point
*/
void
kmain(void)
{
	/* Trigger all init levels */
	trigger_init_levels(INIT_LEVEL_EARLIEST, INIT_LEVEL_LATEST);

	printf("Welcome to ChaOS\n");

	/* Halt (and catch fire) */
	while (42)
		halt();
}

/* Mark the kernel as physically reserved */
NEW_PMM_RESERVED_AREA(kernel, NULL_FRAME, KERNEL_PHYSICAL_END);
