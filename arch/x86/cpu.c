/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <arch/x86/cpu.h>
#include <arch/x86/asm.h>
#include <arch/x86/lapic.h>

/*
** Detects current cpu features, by calling the cpuid instruction.
*/
void
detect_cpu_features(void)
{
	struct cpu *cpu;

	cpu = current_cpu();
	cpuid_string(0x0, cpu->vendor_id);
	cpu->vendor_id[12] = '\0';
	cpuid(0x1, &cpu->signature, &cpu->features);
}

/*
** Returns the current cpu actually running this code.
*/
struct cpu *
current_cpu(void)
{
	uint32 apic_id;

	apic_id = lapic_get_id();
	for (struct cpu *cpu = cpus; cpu < cpus + ncpu; ++cpu)
	{
		if (cpu->lapic_id == apic_id)
			return (cpu);
	}
	panic("Current cpu has an unknown local APIC id");
}
