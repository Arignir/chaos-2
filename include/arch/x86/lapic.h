/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_LAPIC_H_
# define _ARCH_X86_LAPIC_H_

# include <chaosdef.h>
# include <kernel/memory.h>

/*
** Local APIC registers offset
*/
enum lapic_reg
{
	LAPIC_ID		= 0x20,
	LAPIC_VERSION		= 0x30,
};

extern physaddr_t lapic_paddr;

void	lapic_init(void);
uint32	get_lapic_id(void);

#endif /* !_ARCH_X86_LAPIC_H_ */
