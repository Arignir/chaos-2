/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/vaspace.h>
#include <kernel/vseg.h>
#include <kernel/thread.h>
#include <kernel/kalloc.h>
#include <kernel/random.h>
#include <stdio.h>
#include <string.h>

/*
** Initializes the given virtual address space with default values.
**
** Only the kernel's virtual segment is mapped.
*/
status_t
vaspace_init(struct vaspace *vaspace)
{
	memset(vaspace, 0, sizeof(*vaspace));
	spin_rwlock_init(&vaspace->rwlock);
	return (vaspace_add_vseg(vaspace,
		KERNEL_VIRTUAL_BASE,
		(virtaddr_t)ROUND_DOWN(UINTPTR_MAX, PAGE_SIZE),
		VSEG_DEFAULT | VSEG_WRITE | VSEG_EXEC
	));
}

/*
** Creates and maps the given virtual segment to the current virtual address space.
** This asserts the current virtual address space is already locked in writting.
**
** This will check if the new virtual segment is overlapping an existing
** one and throw an error if it's the case.
*/
status_t
vaspace_new_vseg(virtaddr_t start, size_t size, mmap_flags_t flags)
{
	status_t s;
	struct vaspace *vaspace;

	vaspace = current_vaspace();
	assert_vmm(IS_PAGE_ALIGNED(start));
	assert_vmm(IS_PAGE_ALIGNED(size));

	/* Sets size to PAGE_SIZE if size == 0 */
	size += (size == 0) * PAGE_SIZE;

	/* Try to map the segment */
	if (mmap(start, size, flags) != start) {
		return (ERR_CANT_MAP);
	}

	/* Try to add the segment */
	s = vaspace_add_vseg(vaspace, start, (uchar *)start + size - PAGE_SIZE, flags);
	if (s) {
		/* Unmap in case of failure */
		munmap(start, size, MUNMAP_DEFAULT);
	}
	return (s);
}

/*
** If ASLR is enable, this function will add a new vseg of size 'size' to a random
** location.
** If not, this function will put it anywhere it finds enough place.
** Return value points to the start of the virtual segment, or NULL if an error
** occured (not enough memory etc.)
**
** This function will iterate through all vsegs and calculate the length between them.
** It will retain the biggest empty space and the segments associated with it and
** create a new segment of desired size in this empty area.
*/
virtaddr_t
vaspace_new_random_vseg(size_t size, mmap_flags_t flags)
{
	struct vaspace *vaspace;
	size_t biggest_length;
	size_t tmp_length;
	struct vseg null_vseg;
	struct vseg *biggest;
	struct vseg *prev;
	struct vseg *seg;
	size_t i;
	virtaddr_t start;

	biggest_length = 0;
	vaspace = current_vaspace();
	vseg_init(&null_vseg, NULL, NULL, VSEG_DEFAULT);
	prev = biggest = &null_vseg;
	for (i = 0; i < vaspace->vseg_size; ++i) {
		seg = vaspace->vsegs + i;
		tmp_length = vseg_length_between(prev, seg);
		if (tmp_length > biggest_length) {
			biggest = prev;
			biggest_length = tmp_length;
		}
		prev = seg;
	}
	if (biggest_length < size) {
		return (NULL);
	}
#if KCONFIG_ENABLE_ASLR
	biggest_length += PAGE_SIZE;
	start = (uchar *)biggest->end + PAGE_SIZE + rand_uintptr() % (biggest_length - size);
	start = (uchar *)ROUND_DOWN((uintptr)start, PAGE_SIZE);
#else
	start = (uchar *)biggest->end + PAGE_SIZE;
#endif /* KCONFIG_ENABLE_ASLR */
	if (vaspace_new_vseg(start, size, flags)) {
		return (NULL);
	}
	return (start);
}

/*
** Creates and adds a virtual segment based on an existing mapping.
** This asserts the given virtual address space is already locked in writting.
**
** This will check if the new virtual segment is overlapping an existing
** one and throw an error if it's the case.
*/
status_t
vaspace_add_vseg(
	struct vaspace *vaspace,
	virtaddr_t start,
	virtaddr_t end,
	vseg_flags_t flags
)
{
	size_t i;
	struct vseg vseg;
	struct vseg *new_vsegs;

	assert_vmm(IS_PAGE_ALIGNED(start));
	assert_vmm(IS_PAGE_ALIGNED(end));

	vseg_init(&vseg, start, end, flags);
	for (i = 0; i < vaspace->vseg_size; ++i) {
		if (vseg_intersects(vaspace->vsegs + i, &vseg)) {
			return (ERR_ALREADY_MAPPED);
		}
		if (vaspace->vsegs[i].start > vseg.start) {
			break;
		}
	}

	/* Allocates the segment space */
	new_vsegs = krealloc(
		vaspace->vsegs,
		sizeof(*new_vsegs) * (vaspace->vseg_size + 1)
	);
	if (unlikely(new_vsegs == NULL)) {
		return (ERR_NO_MEMORY);
	}

	/* Insertion sort. We move old values away. */
	memmove(
		new_vsegs + (i + 1),
		new_vsegs + i,
		(vaspace->vseg_size - i) * sizeof(*new_vsegs)
	);

	/* And add the new one to the remaining 'i'. */
	new_vsegs[i] = vseg;
	vaspace->vsegs = new_vsegs;
	++vaspace->vseg_size;
	return (OK);
}

/*
** Removes and unmaps the virtual segment with given index of the current
** virtual address space.
** This asserts the current virtual address space is already locked in writting.
**
** The index must be valid or undefined behaviour may occur.
**
** Any pointer or index on the virtual segment list will be invalidate passed
** this function.
** You don't need to release the writer lock of the virtual segment too.
*/
void
vaspace_remove_vseg(size_t idx, munmap_flags_t flags)
{
	struct vaspace *vaspace;
	struct vseg const *seg;
	struct vseg *vsegs;

	vaspace = current_vaspace();
	assert_vmm(idx < vaspace->vseg_size);
	seg = vaspace->vsegs + idx;

	/* Unmap the segment */
	munmap(seg->start, (uchar *)seg->end - (uchar *)seg->start + PAGE_SIZE, flags);

	/* Remove element but conserve order */
	memmove(
		vaspace->vsegs + idx,
		vaspace->vsegs + (idx + 1),
		(vaspace->vseg_size - idx - 1) * sizeof(*vaspace->vsegs)
	);
	vsegs = krealloc(
		vaspace->vsegs,
		sizeof(*vsegs) * (vaspace->vseg_size - 1)
	);
	--vaspace->vseg_size;
	if (vsegs != NULL) {
		vaspace->vsegs = vsegs;
	}
}

/*
** Dumps the given virtual address space.
**
** Assumes the address space is already locked.
*/
void
vaspace_dump(struct vaspace const *vaspace)
{
	struct vseg const *vseg;

	printf("Shared by %u thread(s)\n", vaspace->count);
	printf("Number of vsegs: %zu\n", vaspace->vseg_size);
	for (vseg = vaspace->vsegs; vseg < vaspace->vsegs + vaspace->vseg_size; ++vseg) {
		printf("\tFrom %p to %p (Size: %p) %c%c%c\n",
			vseg->start,
			(uchar *)vseg->end + PAGE_SIZE - 1,
			(uchar *)vseg->end - (uchar *)vseg->start + PAGE_SIZE,
			vseg->flags & VSEG_USER ? 'u' : 'k',
			vseg->flags & VSEG_WRITE ? 'w' : 'r',
			vseg->flags & VSEG_EXEC ? 'x' : '-'
		);
	}
}
