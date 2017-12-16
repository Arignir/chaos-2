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

#endif /* !_ARCH_X86_X86_H_ */
