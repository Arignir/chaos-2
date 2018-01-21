/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/pmm.h>
#include <kernel/kalloc.h>
#include <kernel/cpu.h>
#include <kernel/scheduler.h>
#include <arch/x86/asm.h>
#include <arch/x86/interrupts.h>
#include <arch/x86/apic.h>

static volatile uchar *apic = NULL;

static void	apic_timer_ihandler(struct iframe *iframe);
static void	apic_error_ihandler(struct iframe *iframe);
static void	apic_spurious_ihandler(struct iframe *iframe);

/* Variable to transmit the AP's new stack when starting up */
__section(".boot_memory")
virtaddr_t ap_boot_stack;

/*
** Writes to a local APIC register
*/
static inline void
apic_write(enum apic_reg reg, uint32 value)
{
	*((volatile uint32 *)(apic + reg)) = value;
}

/*
** Reads a local APIC register
*/
static inline uint32
apic_read(enum apic_reg reg)
{
	return (*((volatile uint32 *)(apic + reg)));
}

/*
** Initializes the local APIC.
*/
void
apic_init(void)
{
	uint64 msr;

	assert(apic);

	/* Enable Local APIC by setting spurious interrupt vector */
	apic_write(APIC_SIV, APIC_SVR_ENABLED | INT_APIC_SPURIOUS);

	/* Mask unused/unsupported interrupts */
	apic_write(APIC_LVT_LINT0, APIC_LVT_MASKED);
	apic_write(APIC_LVT_LINT1, APIC_LVT_MASKED);
	apic_write(APIC_LVT_PERFCOUNT, APIC_LVT_MASKED);

	/* Set up apic Timer */
	apic_write(APIC_TIMER_DCR, APIC_TIMER_X8);
	apic_write(APIC_LVT_TIMER, APIC_TIMER_PERIODIC | INT_APIC_TIMER);
	apic_write(APIC_TIMER_ICR, 10000000);

	/* Map error interrupt */
	apic_write(APIC_LVT_ERROR, INT_APIC_ERROR);

	/* IRQs */
	register_int_handler(INT_APIC_TIMER, &apic_timer_ihandler);
	register_int_handler(INT_APIC_ERROR, &apic_error_ihandler);
	register_int_handler(INT_APIC_SPURIOUS, &apic_spurious_ihandler);

	/* Clear Error Status Register */
	apic_write(APIC_ESR, 0x0);
	apic_write(APIC_ESR, 0x0);

	/* Clear any outstanding interrupts */
	apic_write(APIC_EOI, 0x0);

	/* Clear task priority to enable all interrupts */
	apic_write(APIC_TPR, 0x0);

	/* Enable APIC in the MSR */
	msr = read_msr(MSR_APIC_BASE);
	msr |= (1 << 11u);
	write_msr(MSR_APIC_BASE, msr);
}

/*
** Returns the id of the current's processor local APIC.
*/
uint32
apic_get_id(void)
{
	return (apic_read(APIC_ID) >> 24u);
}

/*
** Maps the APIC registers to the given physical address
*/
void
apic_map(physaddr_t pa)
{
	/* Mark the frame of the APIC as allocated */
	mark_range_as_allocated(pa, pa + PAGE_SIZE);

	/* Map it to memory */
	apic = kalloc_device(
		PAGE_SIZE,
		pa
	);
	assert_ne(apic, NULL);
}

/*
** Issue an end-of-interrupt signal so that the apic can process the next
** interrupt.
*/
static void
apic_eoi(void)
{
	apic_write(APIC_EOI, 0x0);
}

/*
** Handler for the timer IRQ
**
** We must send the End Of Interrupt (EOI) before yielding the current thread
** or the next thread will not receive any more APIC interrupt.
*/
void
apic_timer_ihandler(struct iframe *iframe __unused)
{
	apic_eoi();
	if (current_cpu()->thread)
		yield();
}

/*
** Handler for the Spurious interrupt
*/
void
apic_error_ihandler(struct iframe *iframe __unused)
{
	panic("apic error encountered.\n");
	apic_eoi();
}

/*
** Handler for the Spurious interrupt
*/
void
apic_spurious_ihandler(struct iframe *iframe __unused)
{
	panic("apic spurious interruption encountered.\n");
	apic_eoi();
}

/*
** Sends the given IPI to the given local APIC.
*/
void
apic_send_ipi(uint32 dest, uint32 flags)
{
	apic_write(APIC_ICR_HIGH, dest << 24u);
	apic_write(APIC_ICR_LOW, flags);
}

/*
** Poll the delivery status bit till the latest IPI is acked
** by the destination core, or timeout.
**
** FIXME: We should use fine-grained timeout
*/
bool
apic_ipi_acked(void)
{
	size_t timeout;
	uint32 reg;

	timeout = 100;
	while (--timeout > 0) {
		reg = apic_read(APIC_ICR_LOW);
		if (!(reg & APIC_ICR_PENDING))
			return (true);
	}
	return (false);
}

#if KCONFIG_ENABLE_SMP

/*
** Waits for a couple of cpu clocks
*/
static void
micro_wait(void)
{
	uint64 tsc;

	tsc = rdtsc();
	while (rdtsc() < tsc + APIC_INIT_CLOCK_WAIT);
}

/*
** Starts the AP with the given apic id and makes it jump at given address.
*/
status_t
apic_start_ap(struct cpu *ap, uintptr addr)
{
	ushort *wrv;

	assert_eq(addr & 0xFFF00FFF, 0);

	/* Allocate stack for the new cpu */
	ap_boot_stack = kalloc_aligned(PAGE_SIZE * KCONFIG_KERNEL_STACK_SIZE);
	if (ap_boot_stack == NULL) {
		return (ERR_NO_MEMORY);
	}
	ap->scheduler_stack = ap_boot_stack;
	ap_boot_stack = (uchar *)ap_boot_stack + 16 * PAGE_SIZE;
	ap->scheduler_stack_top = ap_boot_stack;

	/*
	** MP Specification says that we must initialize CMOS shutdown code to
	** 0xA and the warm reset vector (DWORD based at 40:67) to point to the
	** AP startup code before doing the universal startup algorithm.
	*/
	outb(PORT_CMOS, 0xF); // Offset 0xF is shutdown code
	outb(PORT_CMOS_RETURN, 0x0A);

	/* Warm reset vector is at fixed address 0x40:0x67 */
	wrv = (ushort *)(P2V(0x40 << 4 | 0x67));
	wrv[1] = 0;
	wrv[0] = addr >> 4;

	/* Universal Startup Algorithm */

	apic_send_ipi(ap->apic_id, APIC_ICR_INIT);
	assert(apic_ipi_acked());
	micro_wait();

	for (int i = 0; i < 2; ++i) {
		apic_send_ipi(ap->apic_id, APIC_ICR_STARTUP | (addr >> 12));
		micro_wait();
	}
	return (OK);
}

#endif /* KCONFIG_ENABLE_SMP */
