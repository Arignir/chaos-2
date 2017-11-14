/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kconfig.h> /* TODO */
#include <arch/x86/smp.h> /* TODO */

#include <kernel/multiboot2.h>
#include <drivers/vga.h>
#include <stdio.h>
#include <string.h>

/*
** Kernel main entry point
*/
void
kmain(uint32 mb_magic, uintptr mb_ptr __unused) /* TODO: use mb_ptr */
{
	vga_set_color(VGA_BLACK, VGA_WHITE);
	vga_clear();

	/* Ensure that we have been booted via a multiboot-compliant bootloader */
	assert_eq(mb_magic, MULTIBOOT2_BOOTLOADER_MAGIC);

	printf("Hello Kernel World!\n");

#if KCONFIG_ENABLE_SMP
	mp_init();
	printf("Nb cpu(s): %u\n", ncpu);
#endif /* KCONFIG_ENABLE_SMP */

	/* Halt */
	while (42);
}
