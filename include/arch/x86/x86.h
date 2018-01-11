/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_X86_H_
# define _ARCH_X86_X86_H_

# define NULL_SELECTOR			0x00
# define KERNEL_CODE_SELECTOR		0x08
# define KERNEL_DATA_SELECTOR		0x10
# define USER_CODE_SELECTOR		0x18
# define USER_DATA_SELECTOR		0x20
# define TSS_SELECTORS			0x28

/*
** An enumeration of all rings level
*/
enum dpl
{
	DPL_RING0			= 0x0,
	DPL_RING1			= 0x1,
	DPL_RING2			= 0x2,
	DPL_RING3			= 0x3,
};

/*
** Flag register possible values.
*/
# define FL_DEFAULT	(0x00000002) /* Default flag value */
# define FL_CF		(0x00000001) /* Carry Flag */
# define FL_PF		(0x00000004) /* Parity Flag */
# define FL_AF		(0x00000010) /* Auxiliary carry Flag */
# define FL_ZF		(0x00000040) /* Zero Flag */
# define FL_SF		(0x00000080) /* Sign Flag */
# define FL_TF		(0x00000100) /* Trap Flag */
# define FL_IF		(0x00000200) /* Interrupt Enable */
# define FL_DF		(0x00000400) /* Direction Flag */
# define FL_OF		(0x00000800) /* Overflow Flag */
# define FL_IOPL_MASK	(0x00003000) /* I/O Privilege Level bitmask */
# define FL_IOPL_0	(0x00000000) /*   IOPL == 0 */
# define FL_IOPL_1	(0x00001000) /*   IOPL == 1 */
# define FL_IOPL_2	(0x00002000) /*   IOPL == 2 */
# define FL_IOPL_3	(0x00003000) /*   IOPL == 3 */
# define FL_NT		(0x00004000) /* Nested Task */
# define FL_RF		(0x00010000) /* Resume Flag */
# define FL_VM		(0x00020000) /* Virtual 8086 mode */
# define FL_AC		(0x00040000) /* Alignment Check */
# define FL_VIF		(0x00080000) /* Virtual Interrupt Flag */
# define FL_VIP		(0x00100000) /* Virtual Interrupt Pending */
# define FL_ID		(0x00200000) /* ID flag */

#endif /* !_ARCH_X86_X86_H_ */
