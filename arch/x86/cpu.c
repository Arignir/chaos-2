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
#include <arch/x86/apic.h>

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
**
** We can't use apic_get_id() if the APIC isn't set up yet, so we're
** using an other structure, 'bsp', which can be used temporarily.
** The function cpu_remap_bsp() will transfer the content of 'bsp'
** to the actuel entry of the BSP within the cpus table.
*/
struct cpu *
current_cpu(void)
{
	uint32 apic_id;

	if (bsp_remapped)
	{
		apic_id = apic_get_id();
		for (struct cpu *cpu = cpus; cpu < cpus + ncpu; ++cpu)
		{
			if (cpu->lapic_id == apic_id)
				return (cpu);
		}
		panic("Current cpu has an unknown local APIC id");
	}
	else {
		return (bsp);
	}
}
