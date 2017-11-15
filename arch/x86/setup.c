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
#include <arch/x86/cpu.h>
#include <arch/x86/smp.h>
#include <arch/x86/asm.h>
#include <drivers/vga.h>
#include <stdio.h>
#include <string.h>

int detect_cpuid(void);

/*
** Early Setup the x86 architecture
*/
void
arch_x86_early_setup(void)
{
	/* Initialize the vga driver early to let us */
	vga_init();

	/* Ensure we have CPUID enable */
	if (!detect_cpuid()) {
		panic("Your CPU must support the CPUID instruction to run ChaOS.");
	}
}

/*
** Setup the x86 architecture
*/
void
arch_x86_setup(void)
{
	struct cpu *cpu;
	bool smp_enabled;

	/* Enable SMP if it's available */
#if KCONFIG_ENABLE_SMP
	smp_enabled = mp_init();
#else
	smp_enabled = false;
#endif /* KCONFIG_ENABLE_SMP */

	/* Else, set the only processor to default values */
	if (!smp_enabled) {
		ncpu = 1;
		cpus[0].conf = NULL;
	}

	printf("Nb cpu(s): %u\n\n", ncpu);
	for (uint i = 0; i < ncpu; ++i)
	{
		cpu = cpus + i;

		/* Gather informations and features of selected cpu */
		cpuid_string(0x0, cpu->vendor_id);
		cpu->vendor_id[12] = '\0';
		cpuid(0x1, &cpu->signature, &cpu->features);

		/* Print these informations */
		printf("processor: %u\n", i);
		printf("vendor_id: %s\n", cpu->vendor_id);
		printf("fpu: %y\n", cpu->features & CPUID_EDX_X87);
		printf("sse: %y\n", cpu->features & CPUID_EDX_SSE);
		printf("apic: %y\n", cpu->features & CPUID_EDX_APIC);
		printf("sse2: %y\n", cpu->features & CPUID_EDX_SSE2);
		printf("\n");
	}
}

NEW_INIT_HOOK(arch_x86_setup, &arch_x86_setup, INIT_LEVEL_ARCH);
NEW_INIT_HOOK(arch_x86_early_setup, &arch_x86_early_setup, INIT_LEVEL_ARCH_EARLY);
