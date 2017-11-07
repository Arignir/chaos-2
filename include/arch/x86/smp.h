/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_SMP_H_
# define _ARCH_X86_SMP_H_

# include <chaosdef.h>
# include <kernel/memory.h>

/* See MultiProcessor Specification v1.4 */

/*
** MP Floating Pointer Structure
** Always starts with the "_MP_" signature.
*/
struct mp
{
	uchar const signature[4];	/* _MP_ */
	physaddr_t const conf_physaddr;	/* Physaddr of the MP config table */
	uchar const length;		/* Length of the table/16. Should be 1 */
	uchar const spec_rev;		/* Revision of the MP spec */
	uchar const checksum;		/* All bytes must add up to 0 */
	uchar const type;		/* MP system config type */
	uchar const imcrp;		/* IMCRP */
	uchar const reserved[3];	/* Reserved. Should be 0 */
} __packed;

static_assert(sizeof(struct mp) == 16);

/*
** MP Configuration Table
** Contains, with others, the physical address of the local APIC.
*/
struct mp_conf
{
	uchar const signature[4];	/* PCMP */
	ushort const length;		/* Table length */
	uchar const spec_rev;		/* Revision of the MP spec */
	uchar const checksum;		/* All bytes must add up to 0 */
	uchar const oem_id[8];		/* String that identifies manufacturer */
	uchar const product_id[12];	/* String that identifies product family */
	physaddr_t const oemtable;	/* Pointer to oem-defined conf table, or 0 */
	ushort const oemlength;		/* Length of oem-defined conf table */
	ushort const entry_count;	/* Number of entries */
	physaddr_t lapic_paddr;		/* Physical address of local APIC */
	ushort xlength;			/* Extended table length */
	uchar xchecksum;		/* All bytes of the xtable must add up to 0*/
	uchar reserved;			/* Reserved. Should be 0 */
} __packed;

static_assert(sizeof(struct mp_conf) == 4 * 11);

struct mp_proc
{
	uchar type;			/* Entry Type */
	uchar lapic_id;			/* Local APIC ID */
	uchar lapic_version;		/* Local APIC version number */
	uchar flags;			/* CPU flags */
	uchar signature[4];		/* CPU signature */
	uint feature;			/* CPU features */
	uchar reserved[8];		/* Reserved. */
} __packed;

static_assert(sizeof(struct mp_proc) == 20);

/*
** The different values that the 'entry type' field of MP Configuration Table
** entries can hold. It is possible that some entries contains other type code.
** If it's the case, they should be ignore.
*/
enum MP_ENTRY_TYPE
{
	MP_PROCESSOR		= 0x0,
	MP_BUS			= 0x1,
	MP_IO_APIC		= 0x2,
	MP_IO_INTERRUPT		= 0x3,
	MP_LOCAL_INTERRUPT	= 0x4,
};

/* Number of CPUs enabled. */
extern uint ncpu;

void	mp_init(void);

#endif /* !_ARCH_X86_SMP_H_ */
