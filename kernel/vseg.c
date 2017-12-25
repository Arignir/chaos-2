/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/vseg.h>
#include <kernel/vaspace.h>
#include <kernel/vmm.h>
#include <kernel/cpu.h>
#include <kernel/thread.h>
#include <arch/linker.h>

/*
** Inits a virtual segment.
*/
void
vseg_init(struct vseg *seg, virtaddr_t start, virtaddr_t end)
{
	seg->start = start;
	seg->end = end;
}

/*
** Grows the given virtual segment by the given size.
**
** size must be page-aligned.
*/
status_t
vseg_grow(struct vseg *seg, size_t size, mmap_flags_t flags)
{
	size_t i;
	struct vaspace *vaspace;
	struct vseg tmp_seg;

	assert_vmm(IS_PAGE_ALIGNED(size));

	vaspace = current_cpu()->thread->vaspace;
	tmp_seg = *seg;
	tmp_seg.end = (char *)seg->end + size;

	/* Tests if the new vseg interesects with an existing one */
	for (i = 0; i < vaspace->vseg_size; ++i) {
		if (&vaspace->vsegs[i] != seg
			&& vseg_intersects(&vaspace->vsegs[i], &tmp_seg)) {
			return (ERR_ALREADY_MAPPED);
		}
	}

	/* Map the new memory */
	if (mmap((uchar *)seg->end + PAGE_SIZE, size, flags)
			!= (uchar *)seg->end + PAGE_SIZE) {
		return (ERR_CANT_MAP);
	}

	/* Update the segment */
	seg->end = tmp_seg.end;
	return (OK);
}

/*
** Reduces the given virtual segment by the given size.
**
** size must be page-aligned.
*/
status_t
vseg_reduce(struct vseg *seg, size_t size, munmap_flags_t flags)
{
	virtaddr_t new_end;

	new_end = (char *)seg->end - size;
	assert_vmm(IS_PAGE_ALIGNED(size));
	assert_vmm(new_end > seg->start);

	munmap((uchar *)new_end + PAGE_SIZE, size, flags);
	seg->end = new_end;
	return (OK);
}

/*
** Returns true if the two given virtual segments intersects.
** This function assumes both given virtual segment are already
** locked.
*/
bool
vseg_intersects(struct vseg *seg1, struct vseg *seg2)
{
	if (seg1->start < seg2->start) {
		return (seg2->start <= seg1->end);
	} else { // seg2->start <= seg1->start
		return (seg1->start <= seg2->end);
	}
}
