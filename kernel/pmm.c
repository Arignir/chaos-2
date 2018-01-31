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
#include <kernel/multiboot.h>
#include <kernel/spinlock.h>
#include <kernel/initrd.h>
#include <string.h>

#if KCONFIG_DEBUG_PMM
# include <stdio.h>
#endif

/*
** This is a pretty naive frame allocator using a bitmap to memorize which
** frames are free and which ones are not.
**
** It can definitely be optimised, but that's not the point at this moment.
*/

static uchar frame_bitmap[FRAME_BITMAP_SIZE];
static size_t next_frame;

/* Protects frame_bitmap and next_frame from concurrent modifications */
static struct spinlock frame_lock = SPINLOCK_DEFAULT;

/*
** Returns true if the given physical address is taken.
** Must be page-aligned
**
** Assumes the frame bitmap is already locked
*/
static inline bool
is_frame_allocated(physaddr_t frame)
{
	assert_pmm(IS_PAGE_ALIGNED(frame));
	return (frame_bitmap[GET_FRAME_IDX(frame)] & (GET_FRAME_MASK(frame)));
}

/*
** Mark a frame as allocated.
**
** Assumes the frame bitmap is already locked
*/
static inline void
mark_frame_as_allocated(physaddr_t frame)
{
	assert_pmm(IS_PAGE_ALIGNED(frame));
	frame_bitmap[GET_FRAME_IDX(frame)] |= GET_FRAME_MASK(frame);
}

/*
** Mark a frame as freed.
**
** Assumes the frame bitmap is already locked
*/
static inline void
mark_frame_as_free(physaddr_t frame)
{
	assert_pmm(IS_PAGE_ALIGNED(frame));
	frame_bitmap[GET_FRAME_IDX(frame)] &= ~GET_FRAME_MASK(frame);
}

/*
** Mark a range of frames as allocated.
** The range is inclusive.
*/
void
mark_range_as_allocated(physaddr_t start, physaddr_t end)
{
	assert_pmm(IS_PAGE_ALIGNED(start));
	assert_pmm(IS_PAGE_ALIGNED(end));

	spinlock_acquire(&frame_lock);
	while (start <= end)
	{
		mark_frame_as_allocated(start);
		start += PAGE_SIZE;
	}
	spinlock_release(&frame_lock);
}

/*
** Mark a range of frames as freed.
*/
void
mark_range_as_free(physaddr_t start, physaddr_t end)
{
	assert_pmm(IS_PAGE_ALIGNED(start));
	assert_pmm(IS_PAGE_ALIGNED(end));

	spinlock_acquire(&frame_lock);
	while (start <= end)
	{
		mark_frame_as_free(start);
		start += PAGE_SIZE;
	}
	next_frame = start;
	spinlock_release(&frame_lock);
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

	spinlock_acquire(&frame_lock);
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
			spinlock_release(&frame_lock);
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
	spinlock_release(&frame_lock);
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

	spinlock_acquire(&frame_lock);

	/* Set the bit corresponding to this frame to 0 */
	frame_bitmap[GET_FRAME_IDX(frame)] &= ~(GET_FRAME_MASK(frame));
	next_frame = GET_FRAME_IDX(frame);

	spinlock_release(&frame_lock);
}

size_t
pmm_count_free_frames(void)
{
	size_t c;
	size_t i;

	c = 0;
	spinlock_acquire(&frame_lock);
	for (i = 0; i < FRAME_BITMAP_SIZE; ++i) {
		if (frame_bitmap[i]) {
			c += !(frame_bitmap[i] & (1 << 0));
			c += !(frame_bitmap[i] & (1 << 1));
			c += !(frame_bitmap[i] & (1 << 2));
			c += !(frame_bitmap[i] & (1 << 3));
			c += !(frame_bitmap[i] & (1 << 4));
			c += !(frame_bitmap[i] & (1 << 5));
			c += !(frame_bitmap[i] & (1 << 6));
			c += !(frame_bitmap[i] & (1 << 7));
		} else {
			c += 8;
		}
	}
	spinlock_release(&frame_lock);
	return (c);
}

extern struct pmm_reserved_area const __start_pmm_reserved_area[] __weak;
extern struct pmm_reserved_area const __stop_pmm_reserved_area[] __weak;

/*
** Inits the physical memory manager
*/
static void
pmm_init(void)
{
	multiboot_memory_map_t const *mmap;
	struct pmm_reserved_area const *pra;
	struct initrd_phys const *initrd;

	/* Mark all memory as allocated */
	memset(frame_bitmap, 0xFF, sizeof(frame_bitmap));

	/* Parse the multiboot structure to unmark available memory */
	mmap = multiboot_infos.mmap;
	while (mmap < multiboot_infos.mmap_end)
	{
		if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			mark_range_as_free(
				ROUND_DOWN(mmap->addr, PAGE_SIZE),
				ALIGN(mmap->addr + mmap->len, PAGE_SIZE)
			);
		}
		mmap = (multiboot_memory_map_t const *)((uchar const *)mmap + multiboot_infos.mmap_entry_size);
	}

	/* Go through all reserved area and mark them as allocated */
	for (pra = __start_pmm_reserved_area; pra < __stop_pmm_reserved_area; ++pra)
	{
#if KCONFIG_DEBUG_PMM
		printf("Marking area \"%s\" as allocated (%p - %p)\n",
				pra->name, (void const *)pra->start, (void const *)pra->end);
#endif /* KCONFIG_DEBUG_PMM */
		mark_range_as_allocated(pra->start, pra->end);
	}

	/* Mark the initrd (if available) as allocated */
	if (initrd_is_present()) {
		initrd = initrd_get_physical();
		mark_range_as_allocated(PAGE_ALIGN(initrd->start), PAGE_ALIGN(initrd->end));
	}

	/* Quickly ensure the kernel has been marked as allocated */
	assert_pmm(is_frame_allocated(ROUND_DOWN(V2P(&pmm_init), PAGE_SIZE)));
}

NEW_INIT_HOOK(pmm_init, &pmm_init, INIT_LEVEL_PMM);
