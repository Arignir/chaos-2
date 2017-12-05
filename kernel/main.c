/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/init.h>
#include <kernel/multiboot2.h>
#include <kernel/interrupts.h>
#include <stdio.h>

/*
** Kernel main entry point
*/
void
kmain(uint32 mb_magic, uintptr mb_ptr __unused) /* TODO: use mb_ptr */
{
	/* Ensure that we have been booted via a multiboot-compliant bootloader */
	assert_eq(mb_magic, MULTIBOOT2_BOOTLOADER_MAGIC);

	/* Trigger all init levels */
	trigger_init_levels(INIT_LEVEL_EARLIEST, INIT_LEVEL_LATEST);

	printf("Welcome to ChaOS\n");

	/* Halt (and catch fire) */
	while (42)
		halt();
}
