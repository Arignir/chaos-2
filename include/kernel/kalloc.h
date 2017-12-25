/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_KALLOC_H_
# define _KERNEL_KALLOC_H_

# include <chaosdef.h>
# include <kconfig.h>
# include <kernel/memory.h>

struct block
{
	bool used;
	size_t size;
	struct block *prev;
};

# if KCONFIG_DEBUG_KALLOC
#  define assert_kalloc(x) assert(x)
# else
#  define assert_kalloc(x)
# endif

virtaddr_t	kalloc(size_t);
virtaddr_t	kalloc_align(size_t);
virtaddr_t	krealloc(virtaddr_t, size_t);
virtaddr_t	kcalloc(size_t, size_t);
void		kfree(virtaddr_t);
void		kalloc_init(void);

static_assert(sizeof(struct block) % sizeof(void *) == 0);

#endif /* !_KERNEL_KALLOC_H_ */

