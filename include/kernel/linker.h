/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_LINKER_H_
# define _KERNEL_LINKER_H_

# include <chaosdef.h>

/*
** Defines some macro to access binary points defined in linker script
*/

/* Kernel virtual & physical addresses */
extern void *__KERNEL_VIRTUAL_BASE __weak;
extern void *__KERNEL_VIRTUAL_END __weak;
extern void *__KERNEL_PHYSICAL_END __weak;

/* All guaranteed to be page-aligned */
# define KERNEL_VIRTUAL_BASE	((void *)&__KERNEL_VIRTUAL_BASE)
# define KERNEL_VIRTUAL_END	((void *)&__KERNEL_VIRTUAL_END)
# define KERNEL_PHYSICAL_END	((uintptr)&__KERNEL_PHYSICAL_END)

#endif /* !_KERNEL_LINKER_H_ */

