/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_TIMER_H_
# define _ARCH_X86_TIMER_H_

# include <chaosdef.h>
# include <arch/x86/interrupts.h>

uint64	now(void);

void	timer_ihandler(void);

#endif /* !_ARCH_X86_TIMER_H_ */
