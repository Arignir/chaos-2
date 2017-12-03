/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_IOAPIC_H_
# define _ARCH_X86_IOAPIC_H_

# include <chaosdef.h>
# include <kernel/memory.h>

# define IOAPIC_BASE_ADDR ((physaddr_t)0x0FEC00000)

/*
** All I/O APIC register offset
*/
enum ioapic_reg
{
	IOAPIC_ID	= 0x00,		/* ID */
	IOAPIC_VERSION	= 0x01,		/* Version */
	IOAPIC_TABLE	= 0x10,		/* Table base */
};

void	ioapic_init(void);
void	ioapic_map(physaddr_t);

#endif /* !_ARCH_X86_IOAPIC_H_ */
