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

	INT_IRQ_TIMER			= 0x20,

	INT_SPURIOUS			= 0xFF,

	X86_INT_MAX			= 0x100,
};

#endif /* !_ARCH_X86_INTERRUPTS_H_ */
