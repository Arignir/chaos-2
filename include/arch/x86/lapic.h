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
	LAPIC_ID		= 0x020, /* ID */
	LAPIC_VERSION		= 0x030, /* Version */
	LAPIC_TPR		= 0x080, /* Task Priority Register */
	LAPIC_APR		= 0x090, /* Arbritation Priority Register */
	LAPIC_PPR		= 0x0A0, /* Processor Priority Register */
	LAPIC_EOI		= 0x0B0, /* End Of Interrupt */
	LAPIC_RRD		= 0x0C0, /* Remote Read Register */
	LAPIC_LDR		= 0x0D0, /* Logical Destination Register */
	LAPIC_DFR		= 0x0E0, /* Destination Format Register */
	LAPIC_SIV		= 0x0F0, /* Spurious Interrupt Vector */
	LAPIC_ESR		= 0x280, /* Error Status Register */
	LAPIC_ICR_LOW		= 0x300, /* Interrupt Command Register - bits 0-31 */
	LAPIC_ICR_HIGH		= 0x310, /* Interrupt Command Register - bits 32-61 */
	LAPIC_LVT_TIMER		= 0x320, /* LVT Timer Register */
	LAPIC_LVT_THERMAL	= 0x330, /* LVT Thermal Sensor Register */
	LAPIC_LVT_PERFCOUNT	= 0x340, /* LVT Performance Counter Register */
	LAPIC_LVT_LINT0		= 0x350, /* LVT LINT0 Register */
	LAPIC_LVT_LINT1		= 0x360, /* LVT LINT1 Register */
	LAPIC_LVT_ERROR		= 0x370, /* LVT Error Register */
	LAPIC_TIMER_ICR		= 0x380, /* Timer Initial Count Register */
	LAPIC_TIMER_CCR		= 0x390, /* Timer Current Count Register */
	LAPIC_TIMER_DCR		= 0x3E0, /* Timer Divide Configuration Register */
};

# define LAPIC_LVT_MASKED	0x00010000
# define LAPIC_SVR_ENABLED	0x00000100

/* Bitfield for the Interrupt Command Register */
# define LAPIC_ICR_FIXED	(0b000 << 8)
# define LAPIC_ICR_LOW		(0b001 << 8)
# define LAPIC_ICR_SMI		(0b010 << 8)
# define LAPIC_ICR_NMI		(0b100 << 8)
# define LAPIC_ICR_INIT		(0b101 << 8)
# define LAPIC_ICR_STARTUP	(0b110 << 8)
# define LAPIC_ICR_PHYSICAL	(0 << 11)
# define LAPIC_ICR_LOGICAL	(1 << 11)
# define LAPIC_ICR_IDLE		(0 << 12)
# define LAPIC_ICR_PENDING	(1 << 12)
# define LAPIC_ICR_DEASSERT	(0 << 14)
# define LAPIC_ICR_ASSERT	(1 << 14)
# define LAPIC_ICR_EDGE		(0 << 15)
# define LAPIC_ICR_LEVEL	(1 << 15)


enum lapic_timer_speed
{
	LAPIC_TIMER_X1		= 0b111,
	LAPIC_TIMER_X2		= 0b000,
	LAPIC_TIMER_X4		= 0b001,
	LAPIC_TIMER_X8		= 0b010,
	LAPIC_TIMER_X16		= 0b011,
	LAPIC_TIMER_X32		= 0b100,
	LAPIC_TIMER_X64		= 0b101,
	LAPIC_TIMER_X128	= 0b110,
};

enum lapic_timer_mode
{
	LAPIC_TIMER_ONESHOT	= 0 << 17,
	LAPIC_TIMER_PERIODIC	= 1 << 17,
};

void	lapic_init(void);
uint32	lapic_get_id(void);
void	lapic_map(physaddr_t);

#endif /* !_ARCH_X86_LAPIC_H_ */
