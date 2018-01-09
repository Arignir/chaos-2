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
#include <kernel/cpu.h>
#include <kernel/kalloc.h>
#include <stdio.h>
#include <string.h>

/*
** Returns the current virtual address space.
** This functions asserts a thread context.
*/
struct vaspace *
current_vaspace(void)
{
	return (current_cpu()->thread->vaspace);
}

/*
** Initializes the given virtual address space with default values.
**
** Only the kernel's virtual segment is mapped.
*/
status_t
vaspace_init(struct vaspace *vaspace)
{
	memset(vaspace, 0, sizeof(*vaspace));
	rwlock_init(&vaspace->rwlock);
	return (vaspace_add_vseg(vaspace,
		KERNEL_VIRTUAL_BASE,
		(virtaddr_t)ROUND_DOWN(UINTPTR_MAX, PAGE_SIZE)
	));
}

/*
** Creates and maps the given virtual segment to the current virtual address space.
** This asserts the current virtual address space is already locked in writting.
**
** This will check if the new virtual segment is overlapping an existing
** one and throw an error if it's the case.
**
*/
status_t
vaspace_new_vseg(virtaddr_t start, size_t size, mmap_flags_t flags)
{
	size_t i;
	struct vaspace *vaspace;
	struct vseg vseg;
	struct vseg *new_vsegs;

	assert_vmm(IS_PAGE_ALIGNED(start));
	assert_vmm(IS_PAGE_ALIGNED(size));

	/* Sets size to PAGE_SIZE if size == 0 */
	size += (size == 0) * PAGE_SIZE;

	vaspace = current_cpu()->thread->vaspace;
	vseg_init(&vseg, start, (uchar *)start + size - PAGE_SIZE);
	for (i = 0; i < vaspace->vseg_size; ++i) {
		if (vseg_intersects(vaspace->vsegs + i, &vseg)) {
			return (ERR_ALREADY_MAPPED);
		}
	}

	/* Pre-allocates the segment space */
	new_vsegs = krealloc(
		vaspace->vsegs,
		sizeof(*vaspace->vsegs) * (vaspace->vseg_size + 1)
	);

	if (unlikely(new_vsegs == NULL)) {
		return (ERR_NO_MEMORY);
	}

	/* Try to map the segment */
	if (mmap(start, size, flags) != start) {
		return (ERR_CANT_MAP);
	}

	/* Add the segment to the virtual address space */
	vaspace->vsegs = new_vsegs;
	vaspace->vsegs[vaspace->vseg_size] = vseg;
	++vaspace->vseg_size;
	return (OK);
}

/*
** Creates and adds a virtual segment based on an existing mapping.
** This asserts the given virtual address space is already locked in writting.
**
** This will check if the new virtual segment is overlapping an existing
** one and throw an error if it's the case.
*/
status_t
vaspace_add_vseg(struct vaspace *vaspace, virtaddr_t start, virtaddr_t end)
{
	size_t i;
	struct vseg vseg;
	struct vseg *new_vsegs;

	assert_vmm(IS_PAGE_ALIGNED(start));
	assert_vmm(IS_PAGE_ALIGNED(end));

	vseg_init(&vseg, start, end);
	for (i = 0; i < vaspace->vseg_size; ++i) {
		if (vseg_intersects(vaspace->vsegs + i, &vseg)) {
			return (ERR_ALREADY_MAPPED);
		}
	}

	/* Allocates the segment space */
	new_vsegs = krealloc(
		vaspace->vsegs,
		sizeof(*vaspace->vsegs) * (vaspace->vseg_size + 1)
	);

	if (unlikely(new_vsegs == NULL)) {
		return (ERR_NO_MEMORY);
	}

	vaspace->vsegs = new_vsegs;
	vaspace->vsegs[vaspace->vseg_size] = vseg;
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
	struct vseg *seg;

	vaspace = current_cpu()->thread->vaspace;
	assert_vmm(idx < vaspace->vseg_size);
	seg = &vaspace->vsegs[idx];

	/* Unmap the segment */
	munmap(seg->start, (uchar *)seg->end - (uchar *)seg->start + PAGE_SIZE, flags);

	/* Swap the last one with the removed one so there is no hole in the array */
	vaspace->vseg_size -= 1;
	vaspace->vsegs[idx] = vaspace->vsegs[vaspace->vseg_size];
	seg = krealloc(
		vaspace->vsegs,
		sizeof(*vaspace->vsegs) * vaspace->vseg_size
	);
	if (seg != NULL) {
		vaspace->vsegs = seg;
	}
}

/*
** Dumps the given virtual address space.
**
** Assumes the address space is already locked.
*/
void
vaspace_dump(struct vaspace *vaspace)
{
	size_t i;

	printf("Shared by %u thread(s)\n", vaspace->count);
	printf("Number of vsegs: %zu\n", vaspace->vseg_size);
	for (i = 0; i < vaspace->vseg_size; ++i) {
		printf("\t%zu: From %p to %p\n",
				i,
				vaspace->vsegs[i].start,
				vaspace->vsegs[i].end
		);
	}
}
