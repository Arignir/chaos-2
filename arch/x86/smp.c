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
#include <arch/x86/smp.h>
#include <arch/x86/cpu.h>
#include <arch/x86/asm.h>
#include <arch/x86/lapic.h>
#include <string.h>
#include <stdio.h>

/* Number of CPUs enabled. */
uint ncpu = 0;
struct cpu cpus[KCONFIG_MAX_CORE] = { 0 };

#if KCONFIG_ENABLE_SMP

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
	conf = (struct mp_conf *)mp->conf_physaddr;
	if (memcmp(conf->signature, "PCMP", 4) || checksum8(conf, conf->length)) {
		return (NULL);
	}
	*mp_ptr = mp;
	return (conf);
}

/*
** Tests if SMP is available. If it's the case, setup and start the other processors.
*/
bool
mp_init(void)
{
	struct mp *mp;
	struct mp_proc *proc;
	struct mp_conf *conf;
	uchar *type;

	conf = mp_config(&mp);
	if (!conf)
		return (false);
	lapic_paddr = conf->lapic_paddr;
	type = (uchar *)(conf + 1);
	while (type < (uchar *)conf + conf->length)
	{
		switch (*type)
		{
		case MP_PROCESSOR:
			proc = (struct mp_proc *)type;
			if (ncpu < KCONFIG_MAX_CORE) {
				cpus[ncpu].lapic_id = proc->lapic_id;
				++ncpu;
			}
			type += sizeof(*proc);
			break;
		case MP_IO_APIC:
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
	/* If IMCRP bit is set, IMCR is present and PIC Mode is implemented. */
	if (mp->imcrp) {
		outb(IO_IMCR_ADDRESS, 0x70);	/* Select IMCR */
		outb(IO_IMCR_DATA, 0x01);	/* Enable APIC */
	}
	return (true);
}

#endif /* KCONFIG_ENABLE_SMP */
