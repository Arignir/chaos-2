/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kconfig.h>
#include <arch/x86/tss.h>
#include <arch/x86/x86.h>
#include <kernel/cpu.h>
#include <string.h>

__aligned(16)
struct tss tss_array[KCONFIG_MAX_CPUS];

extern struct gdt_tss_entry tss_descriptors[];

/*
** Sets up the only tss we're going to use.
** This is called when setting-up x86 architecture.
*/
void
tss_setup(void)
{
	size_t id;
	struct tss *tss;
	struct gdt_tss_entry *gdt_tss_entry;
	uintptr limit;
	uintptr base;

	id = current_cpu_id();
	tss = tss_array + id;
	base = (uintptr)tss;
	limit = (uintptr)sizeof(tss);
	gdt_tss_entry = tss_descriptors + id;

	gdt_tss_entry->limit_low = limit & 0xFFFF;
	gdt_tss_entry->base_low = base & 0xFFFFFF;
	gdt_tss_entry->limit_high = (limit & 0x0F0000) >> 16u;
	gdt_tss_entry->base_high = (base & 0xFF000000) >> 24u;

	memset(tss, 0, sizeof(struct tss));
	tss->esp0 = 0;
	tss->ss0 = KERNEL_DATA_SELECTOR;
	tss->ss1 = 0;
	tss->ss2 = 0;
	tss->eflags = FL_DEFAULT | FL_IOPL_3;

	ltr((TSS_SELECTORS + id * 0x8) | DPL_RING3);
}

void
arch_set_kernel_stack(uintptr ptr)
{
	tss_array[current_cpu_id()].esp0 = ptr;
}
