/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_INITRD_H_
# define _KERNEL_INITRD_H_

# include <chaoserr.h>
# include <chaosdef.h>
# include <kernel/memory.h>

/*
** Structure representing the initial ramdisk position and length in
** virtual memory.
*/
struct initrd_virt
{
	virtaddr_t start;
	size_t len;
};

/*
** Structure representing the initial ramdisk position and length in
** physical memory.
*/
struct initrd_phys
{
	physaddr_t start;
	physaddr_t end;
};

status_t		initrd_map(void);
struct initrd_virt	*initrd_get_virtual(void);
struct initrd_phys	*initrd_get_physical(void);
void			initrd_set_physical(physaddr_t start, physaddr_t end);
bool			initrd_is_present(void);
void			initrd_dump(void);

#endif /* !_KERNEL_INITRD_H_ */
