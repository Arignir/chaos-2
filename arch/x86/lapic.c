/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <arch/x86/interrupts.h>
#include <arch/x86/lapic.h>

static volatile uchar *lapic = NULL;

/*
** Maps the lapic to the given physicall address
*/
void
lapic_map(physaddr_t pa)
{
	/* TODO This must be updated when paging will be enabled */
	lapic = (uchar *)pa;
}

/*
** Writes to a local APIC register
*/
static void
lapic_write(enum lapic_reg reg, uint32 value)
{
	*(uint32 *)(lapic + reg) = value;
}

/*
** Initializes the local APIC.
*/
void
lapic_init(void)
{
	assert(lapic);

	/* Enable Local APIC by setting spurious interrupt vector */
	lapic_write(LAPIC_SIV, LAPIC_SVR_ENABLED | INT_SPURIOUS);

	/* Mask unused/unsupported interrupts */
	lapic_write(LAPIC_LVT_THERMAL, LAPIC_LVT_MASKED);
	lapic_write(LAPIC_LVT_PERFCOUNT, LAPIC_LVT_MASKED);
	lapic_write(LAPIC_LVT_LINT0, LAPIC_LVT_MASKED);
	lapic_write(LAPIC_LVT_LINT1, LAPIC_LVT_MASKED);
	lapic_write(LAPIC_LVT_ERROR, LAPIC_LVT_MASKED);

	/* Set up LAPIC Timer */
	lapic_write(LAPIC_TIMER_DCR, LAPIC_TIMER_X1);
	lapic_write(LAPIC_TIMER_ICR, 10000000);
	lapic_write(LAPIC_LVT_TIMER, LAPIC_TIMER_PERIODIC | INT_IRQ_TIMER);

	/* Clear Error Status Register */
	lapic_write(LAPIC_ESR, 0x0);
	lapic_write(LAPIC_ESR, 0x0);

	/* Clear any outstanding interrupts */
	lapic_write(LAPIC_EOI, 0x0);

	/* Clear task priority to enable all interrupts */
	lapic_write(LAPIC_TPR, 0x0);
}

/*
** Returns the id of the current's processor local APIC.
*/
uint32
lapic_get_id(void)
{
	return (*(uint32 *)(lapic + LAPIC_ID));
}

/*
** Sends the given IPI to the given local APIC.
*/
void
lapic_send_ipi(uint32 dest, uint8 vector, uint32 flags)
{
	lapic_write(LAPIC_ICR_HIGH, dest << 24);
	lapic_write(LAPIC_ICR_LOW, vector | flags);
}
