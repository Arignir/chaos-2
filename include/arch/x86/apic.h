/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_APIC_H_
# define _ARCH_X86_APIC_H_

# include <kconfig.h>
# include <chaosdef.h>
# include <chaoserr.h>
# include <kernel/memory.h>

# define APIC_BASE_ADDR		0xFEE00000

# define APIC_INIT_CLOCK_WAIT	300000000ull

/*
** Local APIC registers offset
*/
enum apic_reg
{
	APIC_ID			= 0x020, /* ID */
	APIC_VERSION		= 0x030, /* Version */
	APIC_TPR		= 0x080, /* Task Priority Register */
	APIC_APR		= 0x090, /* Arbritation Priority Register */
	APIC_PPR		= 0x0A0, /* Processor Priority Register */
	APIC_EOI		= 0x0B0, /* End Of Interrupt */
	APIC_RRD		= 0x0C0, /* Remote Read Register */
	APIC_LDR		= 0x0D0, /* Logical Destination Register */
	APIC_DFR		= 0x0E0, /* Destination Format Register */
	APIC_SIV		= 0x0F0, /* Spurious Interrupt Vector */
	APIC_ESR		= 0x280, /* Error Status Register */
	APIC_ICR_LOW		= 0x300, /* Interrupt Command Register - bits 0-31 */
	APIC_ICR_HIGH		= 0x310, /* Interrupt Command Register - bits 32-61 */
	APIC_LVT_TIMER		= 0x320, /* LVT Timer Register */
	APIC_LVT_THERMAL	= 0x330, /* LVT Thermal Sensor Register */
	APIC_LVT_PERFCOUNT	= 0x340, /* LVT Performance Counter Register */
	APIC_LVT_LINT0		= 0x350, /* LVT LINT0 Register */
	APIC_LVT_LINT1		= 0x360, /* LVT LINT1 Register */
	APIC_LVT_ERROR		= 0x370, /* LVT Error Register */
	APIC_TIMER_ICR		= 0x380, /* Timer Initial Count Register */
	APIC_TIMER_CCR		= 0x390, /* Timer Current Count Register */
	APIC_TIMER_DCR		= 0x3E0, /* Timer Divide Configuration Register */
};

# define APIC_LVT_MASKED	0x00010000
# define APIC_SVR_ENABLED	0x00000100

/* Bitfield for the Interrupt Command Register */
# define APIC_ICR_FIXED	(0b000 << 8)
# define APIC_ICR_LOWPRI	(0b001 << 8)
# define APIC_ICR_SMI		(0b010 << 8)
# define APIC_ICR_NMI		(0b100 << 8)
# define APIC_ICR_INIT		(0b101 << 8)
# define APIC_ICR_STARTUP	(0b110 << 8)
# define APIC_ICR_PHYSICAL	(0 << 11)
# define APIC_ICR_LOGICAL	(1 << 11)
# define APIC_ICR_IDLE		(0 << 12)
# define APIC_ICR_PENDING	(1 << 12)
# define APIC_ICR_DEASSERT	(0 << 14)
# define APIC_ICR_ASSERT	(1 << 14)
# define APIC_ICR_EDGE		(0 << 15)
# define APIC_ICR_LEVEL	(1 << 15)
# define APIC_ICR_BROADCAST	(0x80000)
# define APIC_ICR_DELIVERED	(0x1000)

enum apic_timer_speed
{
	APIC_TIMER_X1		= 0b1011,
	APIC_TIMER_X2		= 0b0000,
	APIC_TIMER_X4		= 0b0001,
	APIC_TIMER_X8		= 0b0010,
	APIC_TIMER_X16		= 0b0011,
	APIC_TIMER_X32		= 0b1000,
	APIC_TIMER_X64		= 0b1001,
	APIC_TIMER_X128	= 0b1010,
};

enum apic_timer_mode
{
	APIC_TIMER_ONESHOT	= 0 << 17,
	APIC_TIMER_PERIODIC	= 1 << 17,
};

void	apic_init(void);
uint32	apic_get_id(void);
void	apic_map(physaddr_t);
void	apic_send_ipi(uint32 dest, uint32 flags);
bool	apic_ipi_acked(void);
void	apic_eoi(void);

#if KCONFIG_ENABLE_SMP
status_t	apic_start_ap(struct cpu *, uintptr addr);
#endif /* KCONFIG_ENABLE_SMP */

#endif /* !_ARCH_X86_APIC_H_ */
