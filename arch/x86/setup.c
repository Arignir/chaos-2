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
#include <arch/x86/lapic.h>
#include <drivers/vga.h>
#include <stdio.h>
#include <string.h>

int detect_cpuid(void);

static void common_setup(void);

/*
** Early Setup the x86 architecture
*/
static void
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
static void
arch_x86_setup(void)
{
	bool smp_enabled;

	/* If SMP is enabled, detect other CPUs */
#if KCONFIG_ENABLE_SMP
	smp_enabled = mp_init();
#else
	smp_enabled = false;
#endif /* KCONFIG_ENABLE_SMP */

	/* Else, set the only processor to default values */
	if (!smp_enabled) {
		ncpu = 1;
		lapic_paddr = (uint32)read_msr(IA32_APIC_BASE);
		lapic_paddr &= 0xFFFFF000;
		cpus[0].lapic_id = get_lapic_id();
	}

	lapic_init(); /* Enable local APIC */
	common_setup(); /* Finish the processor's setup */
}

/*
** Common CPU setup point.
*/
static void
common_setup(void)
{
	struct cpu *cpu;

	cpu = current_cpu();

	/* Detect cpu features */
	detect_cpu_features();

	/* Print informations about this CPU */
	printf("Processor %u started.\n", cpu->lapic_id);
	printf("\tvendor_id: %s\n", cpu->vendor_id);
	printf("\tfpu: %y, sse: %y, apic: %y, sse2: %y\n",
		cpu->features & CPUID_EDX_X87,
		cpu->features & CPUID_EDX_SSE,
		cpu->features & CPUID_EDX_APIC,
		cpu->features & CPUID_EDX_SSE2
	);
}

NEW_INIT_HOOK(arch_x86_setup, &arch_x86_setup, INIT_LEVEL_ARCH);
NEW_INIT_HOOK(arch_x86_early_setup, &arch_x86_early_setup, INIT_LEVEL_ARCH_EARLY);
