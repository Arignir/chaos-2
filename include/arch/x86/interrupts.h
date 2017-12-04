/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_INTERRUPTS_H_
# define _ARCH_X86_INTERRUPTS_H_

# include <chaosdef.h>
# include <kernel/interrupts.h>

/*
** All x86 interrupt vectors.
*/
enum x86_interrupts
{
	INT_DIVISION_BY_ZERO		= 0,
	INT_DEBUG,
	INT_NMI,
	INT_BREAKPOINT,
	INT_OVERFLOW,
	INT_OUT_OF_BOUNDS,
	INT_INVALID_OPCODE,
	INT_DEVICE_NA,
	INT_DOUBLE_FAULT,
	INT_INVALID_TSS			= 0xA,
	INT_SEGMENT_NOT_PRESENT,
	INT_STACK_FAULT,
	INT_GP_FAULT,
	INT_PAGE_FAULT,
	INT_FPU_EXCEPTION		= 0x10,
	INT_ALIGNMENT_CHECK,
	INT_MACHINE_CHECK,
	INT_SIMD_FP_EXCEPTION,
	INT_VIRT_EXCEPTION,
	INT_SECURITY_EXCEPTION		= 0x1E,

	INT_MAX_INTEL			= 0x1F,
	INT_IRQ0			= 0x20,

	INT_APIC_TIMER			= 0x20,
	INT_APIC_ERROR			= 0x33,

	INT_APIC_SPURIOUS		= 0xFF,

	X86_INT_MAX			= 0x100,
};

/*
** This structure represents the registers and all other informations that
** each interrupt handler receives when triggered.
*/
struct iframe
{
	uintptr gs;
	uintptr fs;
	uintptr es;
	uintptr ds;
	uintptr edi;
	uintptr esi;
	uintptr ebp;
	uintptr _popa_esp;
	uintptr ebx;
	uintptr edx;
	uintptr ecx;
	uintptr eax;
	uintptr int_num;
	uintptr err_code;
	uintptr eip;
	uintptr cs;
	uintptr eflags;
	uintptr esp;
	uintptr ss;
};

enum idt_entry_type
{
	IDT_TASK_GATE_32		= 0x5,
	IDT_INTERRUPT_GATE_16		= 0x6,
	IDT_TRAP_GATE_16		= 0x7,
	IDT_INTERRUPT_GATE_32		= 0xE,
	IDT_TRAP_GATE_32		= 0xF,
};

/*
** An entry within the Interrupt Descriptor Table
*/
struct idt_entry
{
	uint16 callback_low;	/* Bits 0..15 of callback */
	uint16 selector;
	uint8 __zero;		/* Reserved */
	uint type		: 5; /* Gate type and storage segment */
	uint dpl		: 2; /* Descriptor Privilege Level */
	uint present		: 1; /* Present bit */
	uint16 callback_high;	/* Bits 16..31 of callback */
} __packed;

static_assert(sizeof(struct idt_entry) == 2 * sizeof(uint32));

void	idt_setup(void);
void	idt_load(void);

#endif /* !_ARCH_X86_INTERRUPTS_H_ */
