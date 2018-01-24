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
# include <kernel/vmm.h>

/*
** Arch-independant flags for virtual segments
** Warning: depending on the architecture, MMAP_WRITE and MMAP_EXEC may be exclusive.
**
** These flags must _NEVER_ overlap mmap's flags values.
*/
# define VSEG_DEFAULT		MMAP_DEFAULT	/* Kernelspace, read-only, no exec */
# define VSEG_USER		MMAP_USER	/* Vseg belongs to user-space */
# define VSEG_WRITE		MMAP_WRITE	/* Vseg is writtable */
# define VSEG_EXEC		MMAP_EXEC	/* Vseg is executable */

typedef mmap_flags_t vseg_flags_t;

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

	vseg_flags_t flags;
};

void		vseg_init(struct vseg *, virtaddr_t, virtaddr_t, vseg_flags_t);
status_t	vseg_grow(struct vseg *seg, size_t size);
status_t	vseg_reduce(struct vseg *seg, size_t size, munmap_flags_t);
bool		vseg_intersects(struct vseg const *seg1, struct vseg const *seg2);
size_t		vseg_length(struct vseg const *seg);
size_t		vseg_length_between(struct vseg const *seg1, struct vseg const *seg2);

#endif /* !_KERNEL_SEGMENT_H_ */
