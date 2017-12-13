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
#include <kernel/pmm.h>
#include <kernel/thread.h>
#include <arch/x86/cpu.h>
#include <arch/x86/smp.h>
#include <arch/x86/asm.h>
#include <arch/x86/pic.h>
#include <arch/x86/apic.h>
#include <arch/x86/ioapic.h>
#include <arch/x86/interrupts.h>
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
	uint32 eax;
	uint32 edx;

	/* Initialize the vga driver early to let us print some debug informations */
	vga_init();

	/* Set up the default IDT */
	idt_setup();

	/* Ensure we have CPUID enable */
	if (!detect_cpuid()) {
		panic("Your CPU must support the CPUID instruction to run ChaOS.");
	}

	/* Ensure the system supports APIC */
	cpuid(0x1, &eax, &edx);
	if (!(edx & CPUID_EDX_APIC)) {
		panic("Your CPU must support APIC");
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
		ioapic_map(IOAPIC_BASE_ADDR);
		apic_map(APIC_BASE_ADDR);
		cpus[0].lapic_id = apic_get_id();
	}

	/* Remap the BSP's information with it's entry within the cpu table */
	cpu_remap_bsp();

	printf("Number of cpus: %u\n", ncpu);

	pic_init();	/* Disable PIC */
	ioapic_init();	/* Enable I/O APIC */
	apic_init();	/* Enable local APIC */

	/* Set the current cpu's thread to be the boot thread */
	current_cpu()->thread = thread_table;

#if KCONFIG_ENABLE_SMP
	mp_start_aps();	/* Start other processors */
#endif /* KCONFIG_ENABLE_SMP */

	common_setup();	/* Finish the processor's setup */
}

#if KCONFIG_ENABLE_SMP

/*
** AP setup point (after beeing booted in boot_ap())
*/
void
ap_setup(void)
{
	apic_init();
	common_setup();

	while (42)
		halt();
}

#endif /* KCONFIG_ENABLE_SMP */

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

	/* Loads the IDT */
	idt_load();

	/* Print informations about this CPU */
	printf("Processor %u started.\n", cpu->lapic_id);
	printf("\tvendor_id: %s\n", cpu->vendor_id);
	printf("\tfpu: %y, sse: %y, apic: %y, sse2: %y\n",
		cpu->features & CPUID_EDX_X87,
		cpu->features & CPUID_EDX_SSE,
		cpu->features & CPUID_EDX_APIC,
		cpu->features & CPUID_EDX_SSE2
	);

	/* Mark this cpu as started */
	xchg((uint *)&cpu->started, true);

	/* Enable the interrupts */
	enable_interrupts();
}

NEW_INIT_HOOK(arch_x86_setup, &arch_x86_setup, INIT_LEVEL_ARCH);
NEW_INIT_HOOK(arch_x86_early_setup, &arch_x86_early_setup, INIT_LEVEL_ARCH_EARLY);
