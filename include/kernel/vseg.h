/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_SEGMENT_H_
# define _KERNEL_SEGMENT_H_

# include <kernel/memory.h>
# include <kernel/rwlock.h>
# include <kernel/vmm.h>

/*
** A virtual memory segment.
**
** Addresses are INCLUSIVES, which means that a segment that is one page
** long has the same 'start' and 'end' addresses.
**
** This represent a fragment of the virtual memory space available.
*/
struct vseg
{
	/* Start address of this virtual segmetn */
	virtaddr_t start;

	/* End address of this virtual segment */
	virtaddr_t end;
};

void		vseg_init(struct vseg *seg, virtaddr_t start, virtaddr_t end);
status_t	vseg_grow(struct vseg *seg, size_t size, mmap_flags_t);
status_t	vseg_reduce(struct vseg *seg, size_t size, munmap_flags_t);
bool		vseg_intersects(struct vseg const *seg1, struct vseg const *seg2);
size_t		vseg_length(struct vseg const *seg);
size_t		vseg_length_between(struct vseg const *seg1, struct vseg const *seg2);

#endif /* !_KERNEL_SEGMENT_H_ */
