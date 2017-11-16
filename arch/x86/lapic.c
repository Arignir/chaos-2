/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <arch/x86/lapic.h>

physaddr_t lapic_paddr = NULL_FRAME;
volatile uchar *lapic = NULL;

/*
** Initializes the local APIC.
*/
void
lapic_init(void)
{
	if (!lapic_paddr)
		return ;

	lapic = (uchar *)lapic_paddr; /* TODO Change this when paging will be available */
}

/*
** Returns the id of the current's processor local APIC.
*/
uint32
get_lapic_id(void)
{
	return (*(uint32 *)(lapic + LAPIC_ID));
}
