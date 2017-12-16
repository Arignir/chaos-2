/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kconfig.h>
#include <kernel/checksum.h>
#include <kernel/cpu.h>
#include <kernel/memory.h>
#include <kernel/vmm.h>
#include <arch/x86/smp.h>
#include <arch/x86/asm.h>
#include <arch/x86/apic.h>
#include <arch/x86/ioapic.h>
#include <string.h>
#include <stdio.h>

#if KCONFIG_ENABLE_SMP

/* Defined in boot_ap.S */
extern void boot_ap(void);
extern void boot_ap_end(void);

/*
** Looks for the MP Floating Pointer Structure from [start; start + len[.
*/
static struct mp *
mp_search_struct(physaddr_t start, size_t len)
{
	uchar *s;
	uchar *e;

	s = P2V(start);
	e = P2V(start + len);
	while (s < e)
	{
		if (memcmp(s, "_MP_", 4) == 0 && checksum8(s, sizeof(struct mp)) == 0) {
			return ((struct mp *)s);
		}
		s += sizeof(struct mp);
	}
	return (NULL);
}

/*
** Looks for the MP Floating Pointer Structure
** According to the spec, it is in one of the following three locations:
**   1) In the first KB of the EBDA
**   2) In the last KB of system base memory
**   3) In the BIOS ROM between 0xF0000 and 0xFFFFF
*/
static struct mp *
mp_search(void)
{
	struct mp *mp;
	uchar *bda;
	physaddr_t p;

	mp = NULL;
	bda = P2V(0x400);
	p = (bda[0x0F] << 8) | (bda[0x0E] << 4);
	if (p != 0) {
		mp = mp_search_struct(p, 1024u);
	} else {
		p = ((bda[0x014] << 8) | (bda[0x13])) * 1024u;
		if (p != 0) {
			mp = mp_search_struct(p - 1024u, 1024u);
		}
	}
	return (mp ? mp : mp_search_struct(0xF0000, 0x10000));
}

/*
** Looks for the MP Configuration Table, if it exists.
** Returns NULL if it can't be found or if it's invalid.
**
** NOTE: Does not support default configurations for now (conf_physaddr == 0)
*/
static struct mp_conf *
mp_config(struct mp **mp_ptr)
{
	struct mp *mp;
	struct mp_conf *conf;

	mp = mp_search();
	if (mp == NULL || mp->conf_physaddr == 0) {
		return (NULL);
	}
	conf = (struct mp_conf *)P2V(mp->conf_physaddr);
	if (memcmp(conf->signature, "PCMP", 4) || checksum8(conf, conf->length)) {
		return (NULL);
	}
	*mp_ptr = mp;
	return (conf);
}

/*
** Tests if SMP is available.
*/
bool
mp_init(void)
{
	struct mp *mp;
	struct mp_proc *proc;
	struct mp_conf *conf;
	struct mp_ioapic *ioapic;
	uchar *type;

	conf = mp_config(&mp);
	if (!conf)
		return (false);
	apic_map(conf->lapic_paddr);
	type = (uchar *)(conf + 1);
	while (type < (uchar *)conf + conf->length)
	{
		switch (*type)
		{
		case MP_PROCESSOR:
			proc = (struct mp_proc *)type;
			if (ncpu < KCONFIG_MAX_CPUS) {
				cpus[ncpu].lapic_id = proc->lapic_id;
				++ncpu;
			}
			type += sizeof(*proc);
			break;
		case MP_IO_APIC:
			ioapic = (struct mp_ioapic *)type;
			ioapic_map(ioapic->addr);
			type += sizeof(*ioapic);
			break;
		case MP_BUS:
		case MP_IO_INTERRUPT:
		case MP_LOCAL_INTERRUPT:
			type += 8;
			break;
		default:
			/* Unknown entries type have their length following */
			type += *(type + 1);
			break;
		}
	}
	return (true);
}

/*
** Start the other processors
*/
void
mp_start_aps(void)
{
	struct cpu *cpu;
	struct cpu *current;

	/*
	** Boot code is size-limited, so we only do a far-jump to boot_ap,
	** where it's more convienent and not restricted.
	*/
	current = current_cpu();
	*(uchar *)(P2V(TRAMPOLINE_START)) = 0xEA; /* Far jump, IP, CS */
	*(ushort *)(P2V(TRAMPOLINE_START + 1)) = (uint16)(uintptr)&boot_ap + 0x10;
	*(ushort *)(P2V(TRAMPOLINE_START + 3)) = 0xFFFF;

	/* Identity map the kernel so that APs can enable paging */
	assert(mmap_device(
			(virtaddr_t)KERNEL_PHYSICAL_LINK,
			KERNEL_PHYSICAL_LINK,
			PAGE_ALIGN(KERNEL_PHYSICAL_END - KERNEL_PHYSICAL_LINK),
			MMAP_EXEC
	));

	/* Start all available cpus */
	for (cpu = cpus; cpu < cpus + ncpu; ++cpu)
	{
		if (cpu == current)
			continue;
		apic_start_ap(cpu->lapic_id, TRAMPOLINE_START);
		while (!cpu->started);
	}

	/* Unmap the previous mapping */
	munmap(
			(virtaddr_t)KERNEL_PHYSICAL_LINK,
			PAGE_ALIGN(KERNEL_PHYSICAL_END - KERNEL_PHYSICAL_LINK),
			MUNMAP_DONTFREE
	);
}

#endif /* KCONFIG_ENABLE_SMP */
