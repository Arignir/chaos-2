/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_SYSCALL_H_
# define _ARCH_X86_SYSCALL_H_

# include <kernel/syscall.h>
# include <arch/x86/interrupts.h>

void	syscall_init(void);

#endif /* !_ARCH_X86_SYSCALL_H_ */
