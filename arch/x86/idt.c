/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/interrupts.h>
#include <arch/x86/interrupts.h>
#include <arch/x86/x86.h>
#include <arch/x86/asm.h>
#include <string.h>

/* Interrupt Descriptor Table */
static struct idt_entry idt[X86_INT_MAX] = { 0 };

/* Generated in idt_handler.S */
extern void x86_division_by_zero_ihandler(struct iframe *);
extern void x86_debug_ihandler(struct iframe *);
extern void x86_nmi_ihandler(struct iframe *);
extern void x86_breakpoint_ihandler(struct iframe *);
extern void x86_overflow_ihandler(struct iframe *);
extern void x86_out_of_bounds_ihandler(struct iframe *);
extern void x86_invalid_opcode_ihandler(struct iframe *);
extern void x86_device_na_ihandler(struct iframe *);
extern void x86_double_fault_ihandler(struct iframe *);
extern void x86_fpu_segment_overrun_ihandler(struct iframe *);
extern void x86_invalid_tss_ihandler(struct iframe *);
extern void x86_segment_not_present_ihandler(struct iframe *);
extern void x86_stack_fault_ihandler(struct iframe *);
extern void x86_gp_fault_ihandler(struct iframe *);
extern void x86_page_fault_ihandler(struct iframe *);
extern void x86_unknown_int_ihandler(struct iframe *);
extern void x86_fpu_exception_ihandler(struct iframe *);
extern void x86_alignment_check_ihandler(struct iframe *);
extern void x86_machine_check_ihandler(struct iframe *);
extern void x86_simd_fd_exception_ihandler(struct iframe *);
extern void x86_virt_exception_ihandler(struct iframe *);

extern void x86_apic_timer_ihandler(struct iframe *);
extern void x86_apic_error_ihandler(struct iframe *);
extern void x86_apic_spurious_ihandler(struct iframe *);

extern void x86_unknown_ihandler(struct iframe *);

/*
**
** A set of function to manipulate an IDT entry
**
*/

/*
** Sets the interrupt callback for the given idt entry
*/
static inline void
idt_set_callback(struct idt_entry *entry, uintptr ep)
{
	entry->callback_low = (uint16)(ep & 0x0000FFFF);
	entry->callback_high = (uint16)(ep >> 16u);
}

/*
** Updates an IDT entry with the given parameters
*/
static inline void
idt_set_vector(uint8 vec,
		uintptr callback,
		uint16 sel,
		enum dpl dpl,
		enum idt_entry_type type)
{
	struct idt_entry *entry;

	entry = idt + vec;
	memset(entry, 0, sizeof(*entry));
	entry->present = true;
	entry->dpl = dpl;
	entry->type = type;
	entry->selector = sel;
	idt_set_callback(entry, callback);
}

/*
**
** Implementation of arch-independant functions defined in <kernel/interrupts.h>
**
*/

/*
** Enables interrupts
*/
void
enable_interrupts(void)
{
	sti();
}

/*
** Disables interrupts
*/
void
disable_interrupts(void)
{
	cli();
}

/*
** Stores in 'save' the current interrupt state
*/
void
push_interrupt_state(void *save)
{
	*(uint32 *)save = get_eflags().IF;
}

/*
** Restores the current interrupt state
*/
void
pop_interrupt_state(void *save)
{
	union eflags e;

	e = get_eflags();
	e.IF = *(bool *)save;
	set_eflags(e.value);
}

/*
** Sets up the idt
*/
void
idt_setup(void)
{
	size_t i;

	for (i = 0; i < X86_INT_MAX; ++i) {
		idt_set_vector(
				i,
				(uintptr)&x86_unknown_ihandler,
				KERNEL_CODE_SELECTOR,
				DPL_RING0,
				IDT_INTERRUPT_GATE_32
		);
	}

	/* Exceptions */
	idt_set_callback(idt + INT_DIVISION_BY_ZERO,
			(uintptr)&x86_division_by_zero_ihandler);
	idt_set_callback(idt + INT_DEBUG,
			(uintptr)&x86_debug_ihandler);
	idt_set_callback(idt + INT_NMI,
			(uintptr)&x86_nmi_ihandler);
	idt_set_callback(idt + INT_BREAKPOINT,
			(uintptr)&x86_breakpoint_ihandler);
	idt_set_callback(idt + INT_OVERFLOW,
			(uintptr)&x86_overflow_ihandler);
	idt_set_callback(idt + INT_OUT_OF_BOUNDS,
			(uintptr)&x86_out_of_bounds_ihandler);
	idt_set_callback(idt + INT_INVALID_OPCODE,
			(uintptr)&x86_invalid_opcode_ihandler);
	idt_set_callback(idt + INT_DEVICE_NA,
			(uintptr)&x86_device_na_ihandler);
	idt_set_callback(idt + INT_DOUBLE_FAULT,
			(uintptr)&x86_double_fault_ihandler);
	idt_set_callback(idt + INT_INVALID_TSS,
			(uintptr)&x86_invalid_tss_ihandler);
	idt_set_callback(idt + INT_SEGMENT_NOT_PRESENT,
			(uintptr)&x86_segment_not_present_ihandler);
	idt_set_callback(idt + INT_STACK_FAULT,
			(uintptr)&x86_stack_fault_ihandler);
	idt_set_callback(idt + INT_GP_FAULT,
			(uintptr)&x86_gp_fault_ihandler);
	idt_set_callback(idt + INT_PAGE_FAULT,
			(uintptr)&x86_page_fault_ihandler);
	idt_set_callback(idt + INT_FPU_EXCEPTION,
			(uintptr)&x86_fpu_exception_ihandler);
	idt_set_callback(idt + INT_ALIGNMENT_CHECK,
			(uintptr)&x86_alignment_check_ihandler);
	idt_set_callback(idt + INT_MACHINE_CHECK,
			(uintptr)&x86_machine_check_ihandler);
	idt_set_callback(idt + INT_SIMD_FP_EXCEPTION,
			(uintptr)&x86_simd_fd_exception_ihandler);
	idt_set_callback(idt + INT_VIRT_EXCEPTION,
			(uintptr)&x86_virt_exception_ihandler);

	/* IRQs */
	idt_set_callback(idt + INT_APIC_TIMER, (uintptr)&x86_apic_timer_ihandler);
	idt_set_callback(idt + INT_APIC_ERROR, (uintptr)&x86_apic_error_ihandler);
	idt_set_callback(idt + INT_APIC_SPURIOUS, (uintptr)&x86_apic_spurious_ihandler);
}

/*
** Loads the Interrupt Descriptor Table
*/
void
idt_load(void)
{
	lidt(idt, sizeof(idt) - 1);
}
