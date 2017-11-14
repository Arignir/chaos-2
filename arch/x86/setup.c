/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kconfig.h>
#include <kernel/init.h>
#include <arch/x86/smp.h>
#include <drivers/vga.h>
#include <stdio.h>

/*
** Early Setup the x86 architecture
*/
void
arch_x86_early_setup(void)
{
	/* Initialize the vga driver early to let us */
	vga_init();
}

/*
** Setup the x86 architecture
*/
void
arch_x86_setup(void)
{
	/* Enable SMP if it's available */
#if KCONFIG_ENABLE_SMP
	mp_init();
	printf("Nb cpu(s): %u\n", ncpu);
#endif /* KCONFIG_ENABLE_SMP */
}

NEW_INIT_HOOK(arch_x86_setup, &arch_x86_setup, INIT_LEVEL_ARCH);
NEW_INIT_HOOK(arch_x86_early_setup, &arch_x86_early_setup, INIT_LEVEL_ARCH_EARLY);
