/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_PMM_H_
# define _KERNEL_PMM_H_

# include <kconfig.h>
# include <kernel/memory.h>
# include <limits.h>

# define NB_FRAMES		((UINTPTR_MAX / PAGE_SIZE) + 1)
# define FRAME_BITMAP_SIZE	(NB_FRAMES / 8u)

# define GET_FRAME_IDX(x)	(((x) >> 12u) / 8u)
# define GET_FRAME_MASK(x)	(1 << (((x) >> 12u) % 8u))

/* assert_pmm() will assert only if KCONFIG_DEBUG_PMM is defined */
# if KCONFIG_DEBUG_PMM
#  define assert_pmm(x)		assert(x)
# else
#  define assert_pmm(x)
# endif /* KCONFIG_DEBUG_PMM */

physaddr_t	alloc_frame(void);
void		free_frame(physaddr_t frame);
void		mark_range_as_allocated(physaddr_t start, physaddr_t end);
void		mark_range_as_free(physaddr_t start, physaddr_t end);

struct pmm_reserved_area
{
	char const *name;
	physaddr_t start;
	physaddr_t end;
};

# define NEW_PMM_RESERVED_AREA(n, s, e) \
	__aligned(sizeof(void *)) __used __section("pmm_reserved_area")	\
	static struct pmm_reserved_area const _pmm_reserved_area_##n = {	\
		.name = #n,							\
		.start = (s),							\
		.end = (e),							\
	}

#endif /* !_KERNEL_PMM_H_ */
