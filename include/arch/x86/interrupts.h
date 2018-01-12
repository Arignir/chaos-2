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

	INT_PIT				= 0x20,
	INT_APIC_TIMER			= 0x32,
	INT_APIC_ERROR			= 0x33,

	INT_SYSCALL			= 0x80,

	INT_APIC_SPURIOUS		= 0xFF,

	X86_INT_MAX			= 0xFF,
	X86_INT_NB			= 0x100,
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

void	idt_setup(void);
void	idt_load(void);
void	interrupts_handler(struct iframe *iframe);
void	syscall_handler(struct iframe *iframe);

typedef void(*ihandler_t)(struct iframe *);

#endif /* !_ARCH_X86_INTERRUPTS_H_ */
