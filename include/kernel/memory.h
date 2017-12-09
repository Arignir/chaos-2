/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_MEMORY_H_
# define _KERNEL_MEMORY_H_

# include <chaosdef.h>
# include <kconfig.h>

typedef uintptr			physaddr_t;
typedef void			*virtaddr_t;

# define P2V(phys)		((virtaddr_t)(phys))
# define V2P(phys)		((physaddr_t)(phys))

/* The NULL frame. The equivalent of NULL, but for physical memory. */
# define NULL_FRAME		((physaddr_t)0u)

/* A physical memory area reserved for a specific device */
struct pmm_reserved_area
{
	char const *name;
	physaddr_t start;
	physaddr_t end;
};

/* A macro to easily create a new reserved memory area */
# define NEW_PMM_RESERVED_AREA(n, s, e) \
	__aligned(sizeof(void *)) __used __section("pmm_reserved_area")	\
	static struct pmm_reserved_area const _pmm_reserved_area_##n = {	\
		.name = #n,							\
		.start = (s),							\
		.end = (e),							\
	}

#endif /* !_KERNEL_MEMORY_H_ */
