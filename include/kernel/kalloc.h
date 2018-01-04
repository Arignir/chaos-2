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

/* Debug assertions for kernel heap */
# if KCONFIG_DEBUG_KALLOC
#  define assert_kalloc(x) assert(x)
# else
#  define assert_kalloc(x)
# endif /* KCONFIG_DEBUG_KALLOC */

/*
** Each allocation made with kalloc() belongs to a block, which form a linked list.
*/
struct block
{
	bool used;
	size_t size;
	struct block *prev;
};

/*
** We are using a dirty trick to have a quick and functional kalloc_aligned().
** It needs to store the original pointer returned by kalloc() before aligning it to
** a page boundary.
*/
struct aligned_address
{
	virtaddr_t ptr;
	virtaddr_t ori_ptr;
};

virtaddr_t	kalloc(size_t);
virtaddr_t	kalloc_aligned(size_t);
virtaddr_t	kalloc_device(size_t size, physaddr_t pa);
virtaddr_t	krealloc(virtaddr_t, size_t);
virtaddr_t	kcalloc(size_t, size_t);
void		kfree(virtaddr_t);
void		kalloc_init(void);

static_assert(sizeof(struct block) % sizeof(void *) == 0);

#endif /* !_KERNEL_KALLOC_H_ */
