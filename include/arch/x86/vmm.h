/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_VMM_H_
# define _ARCH_X86_VMM_H_

# include <kernel/vmm.h>
# include <chaosdef.h>

/*
** An entry in the page directory
*/
struct			pagedir_entry
{
	union
	{
		struct
		{
			uint32 present : 1;	/* Present in memory */
			uint32 rw : 1;		/* 0 => Readonly / 1 => Readwrite */
			uint32 user : 1;	/* 0 => Kernel page / 1 => User */
			uint32 wtrough : 1;	/* 1 => write through caching */
			uint32 cache : 1;	/* 1 => Cache disable */
			uint32 accessed : 1;	/* set by cpu when accessed */
			uint32 _zero : 1;	/* Must be 0 */
			uint32 size : 1;	/* 0 => 4KiB page, 1 => 4MiB page */
			uint32 __unusued : 4;	/* unused & reserved bits */
			uint32 frame : 20;	/* Frame address */
		};
		uintptr value;
	};
};

/*
** An entry in a page table
*/
struct			pagetable_entry
{
	union
	{
		struct
		{
			uint32 present : 1;	/* Present in memory */
			uint32 rw : 1;		/* 0 => Readonly / 1 => Readwrite */
			uint32 user : 1;	/* 0 => Kernel page / 1 => User */
			uint32 wtrough : 1;	/* 1 => write through caching */
			uint32 cache : 1;	/* 1 => Cache disable */
			uint32 accessed : 1;	/* set by cpu when accessed */
			uint32 dirty : 1;	/* Set by cpu when writting */
			uint32 _zero : 1;	/* Must be zero */
			uint32 global : 1;	/* Prevent tlb update */
			uint32 __unusued : 3;	/* unused & reserved bits */
			uint32 frame : 20;	/* Frame address */
		};
		uintptr value;
	};
};

/*
** A page table, composed of 1024 entries.
*/
struct			page_table
{
	struct pagetable_entry entries[PAGE_SIZE / sizeof(struct pagetable_entry)];
};

/*
** A page directory, composed of 1024 entries.
*/
struct			page_dir
{
	struct pagedir_entry entries[PAGE_SIZE / sizeof(struct pagedir_entry)];
};

/*
** Static assertions in case the compiler fucked up.
*/
static_assert(sizeof(struct pagedir_entry) == sizeof(uintptr));
static_assert(sizeof(struct pagetable_entry) == sizeof(uintptr));
static_assert(sizeof(struct page_table) == PAGE_SIZE);
static_assert(sizeof(struct page_dir) == PAGE_SIZE);

/*
** Returns the address of the current page directory
** This takes advantages of the recursive mapping we set up.
*/
static inline struct page_dir *
get_pagedir(void)
{
	return ((struct page_dir *)0xFFFFF000ul);
}

/*
** Returns the address of the page table at index 'x'.
** This takes advantages of the recursive mapping we set up.
*/
static inline struct page_table *
get_pagetable(uint x)
{
	return ((struct page_table *)(0xFFC00000ul | (((x) & 0x3FF) << 12u)));
}

/*
** Returns the index within the page directory of the page table containg the
** given address.
*/
static inline uint
get_pd_idx(virtaddr_t va)
{
	return ((uintptr)va >> 22u);
}

/*
** Returns the index within the page table of the page the
** given address belongs to.
*/
static inline uint
get_pt_idx(virtaddr_t va)
{
	return (((uintptr)(va) >> 12u) & 0x3FF);
}

/*
** Returns the address of the page with given page directory index
** and given page table index.
*/
static inline virtaddr_t
get_virtaddr(uint pdidx, uint ptidx)
{
	return ((virtaddr_t)((pdidx) << 22u | (ptidx) << 12u));
}

void	vmm_unmap_pagetables(void);
void	vmm_copy_pagetables(struct page_dir *pd);

#endif /* !_ARCH_X86_VMM_H_ */

