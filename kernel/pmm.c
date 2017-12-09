/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/init.h>
#include <kernel/pmm.h>
# include <string.h>

#if KCONFIG_DEBUG_PMM
# include <stdio.h>
#endif

/*
** This is a pretty naive frame allocator using a bitmap to memorize which
** frames are free and which ones are not.
**
** It can definitely be optimised, but that's not the point at this moment.
*/

uchar frame_bitmap[FRAME_BITMAP_SIZE];
static size_t next_frame;

/*
** Returns true if the given physical address is taken.
** Must be page-aligned
*/
static inline bool
is_frame_allocated(physaddr_t frame)
{
	assert_pmm(IS_PAGE_ALIGNED(frame));
	return (frame_bitmap[GET_FRAME_IDX(frame)] & (GET_FRAME_MASK(frame)));
}

/*
** Mark a frame as allocated.
*/
static inline void
mark_frame_as_allocated(physaddr_t frame)
{
	assert_pmm(IS_PAGE_ALIGNED(frame));
	frame_bitmap[GET_FRAME_IDX(frame)] |= GET_FRAME_MASK(frame);
}

/*
** Mark a frame as freed.
*/
static inline void
mark_frame_as_free(physaddr_t frame)
{
	assert_pmm(IS_PAGE_ALIGNED(frame));
	frame_bitmap[GET_FRAME_IDX(frame)] &= ~GET_FRAME_MASK(frame);
}

/*
** Mark a range of frames as allocated.
*/
static void
mark_range_as_allocated(physaddr_t start, physaddr_t end)
{
	assert_pmm(IS_PAGE_ALIGNED(start));
	assert_pmm(IS_PAGE_ALIGNED(end));

	while (start <= end)
	{
		mark_frame_as_allocated(start);
		start += PAGE_SIZE;
	}
}

/*
** Mark a range of frames as freed.
*/
static void
mark_range_as_free(physaddr_t start, physaddr_t end)
{
	assert_pmm(IS_PAGE_ALIGNED(start));
	assert_pmm(IS_PAGE_ALIGNED(end));

	while (start <= end)
	{
		mark_frame_as_free(start);
		start += PAGE_SIZE;
	}
	next_frame = start;
}

/*
** Allocates a new frame and returns it, or NULL_FRAME if there is no physical
** memory left.
**
** The idea is that 'next_frame' contains the index in frame_bitmap of our first
** looking address, the one most likely to be free.
** The search is done in two steps, the first one from 'next_frame' to NB_FRAMES, and
** the second one from 0 to 'next_frame'. If after these two steps no
** free frame was found, then NULL_FRAME is returned. In the other case,
** it also sets 'next_frame' to the index of the following address.
*/
physaddr_t
alloc_frame(void)
{
	size_t i;
	size_t j;
	size_t final;
	bool pass;

	i = next_frame;
	final = FRAME_BITMAP_SIZE;
	pass = false;
look_for_address:
	while (i < final)
	{
		if (frame_bitmap[i] != 0xFFu) {
			j = 0u;
			while (j < 7u && (frame_bitmap[i] & (1 << j)) != 0) {
				++j;
			}
			frame_bitmap[i] |= (1 << j);
			next_frame = i;
			return (PAGE_SIZE * (i * 8u + j));
		}
		++i;
	}
	if (!pass)
	{
		final = next_frame;
		i = 0;
		pass = true;
		goto look_for_address;
	}
	return (NULL_FRAME);
}

/*
** Frees a given frame.
*/
void
free_frame(physaddr_t frame)
{
	/* Ensure the address is page-aligned */
	assert_pmm(IS_PAGE_ALIGNED(frame));

	/* Ensure the given physical address is taken */
	assert_pmm(is_frame_allocated(frame));

	/* Set the bit corresponding to this frame to 0 */
	frame_bitmap[GET_FRAME_IDX(frame)] &= ~(GET_FRAME_MASK(frame));
	next_frame = GET_FRAME_IDX(frame);
}

extern struct pmm_reserved_area const __start_pmm_reserved_area[] __weak;
extern struct pmm_reserved_area const __stop_pmm_reserved_area[] __weak;

/*
** Inits the physical memory manager
*/
static void
pmm_init(void)
{
	struct pmm_reserved_area const *pra;

	/* Mark all memory as allocated */
	memset(frame_bitmap, 0x00, sizeof(frame_bitmap));

	/* Go through all reserved area and mark them as allocated */
	for (pra = __start_pmm_reserved_area; pra < __stop_pmm_reserved_area; ++pra)
	{
#if KCONFIG_DEBUG_PMM
		printf("Marking area \"%s\" as allocated (%p - %p)\n",
				pra->name, pra->start, pra->end);
#endif /* KCONFIG_DEBUG_PMM */
		mark_range_as_allocated(pra->start, pra->end);
	}

	/* Quickly ensure the kernel has been marked as allocated */
	assert_pmm(is_frame_allocated(ROUND_DOWN(V2P(&pmm_init), PAGE_SIZE)));
}

NEW_INIT_HOOK(pmm_init, &pmm_init, INIT_LEVEL_PMM);
