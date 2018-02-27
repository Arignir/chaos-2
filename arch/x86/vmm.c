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
#include <kernel/kalloc.h>
#include <arch/x86/vmm.h>
#include <arch/x86/asm.h>
#include <string.h>
#include <stdio.h>

/*
** WARNING:
**
** All the functions in this file are basic primitives that do not try to lock any mutex.
** More high-level code should do it instead, like the mmap() function.
*/

/*
** Used for debugging purposes. Dumps the page table at given index.
*/
static void
vmm_dump_pagetable(uint pd_idx)
{
	struct page_table const *pt;
	uint j;

	j = 0;
	pt = get_pagetable(pd_idx);
	while (j < 1024u)
	{
		if (pt->entries[j].present) {
			printf("\t[%4u] [%p -> %p] %s %c %c %c %c %c\n",
				j,
				(virtaddr_t)((pd_idx << 22u) | (j << 12u)),
				(virtaddr_t)(pt->entries[j].frame << 12ul),
				"RO\0RW" + pt->entries[j].rw * 3,
				"ku"[pt->entries[j].user],
				"-w"[pt->entries[j].wtrough],
				"-d"[pt->entries[j].cache],
				"-a"[pt->entries[j].accessed],
				"-d"[pt->entries[j].dirty]
			);
		}
		++j;
	}
}

/*
** Used for debugging purposes. Dumps the current page directory.
*/
void
vmm_dump_mem(void)
{
	uint i;
	struct page_dir const *pd;

	pd = get_pagedir();
	for (i = 0; i < 1024u; ++i)
	{
		if (pd->entries[i].present)
		{
			printf("[%4u] [%p -> %p] %s %c %c %c %c %c\n",
				i,
				(virtaddr_t)(i << 22u),
				(virtaddr_t)(pd->entries[i].frame << 12ul),
				"RO\0RW" + pd->entries[i].rw * 3,
				"ku"[pd->entries[i].user],
				"-w"[pd->entries[i].wtrough],
				"-d"[pd->entries[i].cache],
				"-a"[pd->entries[i].accessed],
				"-H"[pd->entries[i].size]
			);
			if (i != 1023) {
				vmm_dump_pagetable(i);
			}
		}
	}
}

/*
** Used for debugging purposes. Dumps the current page directory except kernel space.
*/
void
vmm_dump_user_mem(void)
{
	uint i;
	struct page_dir const *pd;

	pd = get_pagedir();
	for (i = 0; i < get_pd_idx(KERNEL_VIRTUAL_BASE); ++i)
	{
		if (pd->entries[i].present)
		{
			printf("[%4u] [%p -> %p] %s %c %c %c %c %c\n",
				i,
				(virtaddr_t)(i << 22u),
				(virtaddr_t)(pd->entries[i].frame << 12ul),
				"RO\0RW" + pd->entries[i].rw * 3,
				"ku"[pd->entries[i].user],
				"-w"[pd->entries[i].wtrough],
				"-d"[pd->entries[i].cache],
				"-a"[pd->entries[i].accessed],
				"-H"[pd->entries[i].size]
			);
			vmm_dump_pagetable(i);
		}
	}
}

/*
** Prints all pages holding this physical address.
*/
void
vmm_dump_owners(physaddr_t pa)
{
	uchar *va;

	va = NULL;
	printf("Dumping pages holding %p...\n", pa);
	while (va < (uchar *)get_pagedir()) {
		if (vmm_get_frame(va) == pa) {
			printf("\t%p\n", va);
		}
		va += PAGE_SIZE;
	}
	printf("Done\n");
}

/*
** Tells if the given virtual address is mapped.
*/
bool
vmm_is_mapped(const_virtaddr_t va)
{
	struct pagedir_entry const *pde;
	struct pagetable_entry const *pte;

	assert_vmm(IS_PAGE_ALIGNED(va));
	pde = get_pagedir()->entries + get_pd_idx(va);
	if (!pde->present) {
		return (false);
	}
	pte = get_pagetable(get_pd_idx(va))->entries + get_pt_idx(va);
	return (pte->present);
}

/*
** Returns true if the given virtual address is mapped and belongs to userspace.
*/
bool
vmm_is_mapped_in_userspace(const_virtaddr_t va)
{
	struct pagedir_entry const *pde;
	struct pagetable_entry const *pte;

	assert_vmm(IS_PAGE_ALIGNED(va));
	pde = get_pagedir()->entries + get_pd_idx(va);
	if (!pde->present) {
		return (false);
	}
	pte = get_pagetable(get_pd_idx(va))->entries + get_pt_idx(va);
	return (pte->present && pte->user);
}

/*
** Returns true if the given virtual address is mapped and belongs to userspace.
*/
bool
vmm_is_mapped_in_kernelspace(const_virtaddr_t va)
{
	struct pagedir_entry const *pde;
	struct pagetable_entry const *pte;

	assert_vmm(IS_PAGE_ALIGNED(va));
	pde = get_pagedir()->entries + get_pd_idx(va);
	if (!pde->present) {
		return (false);
	}
	pte = get_pagetable(get_pd_idx(va))->entries + get_pt_idx(va);
	return (pte->present && !pte->user);
}

/*
** Returns the frame that the given address is mapped to, or NULL_FRAME
** if it's not mapped.
*/
physaddr_t
vmm_get_frame(virtaddr_t va)
{
	struct pagedir_entry const *pde;
	struct pagetable_entry const *pte;

	assert_vmm(IS_PAGE_ALIGNED(va));
	pde = get_pagedir()->entries + get_pd_idx(va);
	if (!pde->present) {
		return (NULL_FRAME);
	}
	pte = get_pagetable(get_pd_idx(va))->entries + get_pt_idx(va);
	return (pte->frame << 12u);
}

/*
** Maps a physical address to a virtual one.
*/
status_t
vmm_map_physical(virtaddr_t va, physaddr_t pa, mmap_flags_t flags)
{
	struct pagedir_entry *pde;
	struct pagetable_entry *pte;
	struct page_table *pt;

	assert_vmm(IS_PAGE_ALIGNED(va));
	assert_vmm(IS_PAGE_ALIGNED(pa));
	pde = get_pagedir()->entries + get_pd_idx(va);
	pt = get_pagetable(get_pd_idx(va));
	if (pde->present == false)
	{
		pde->value = alloc_frame();
		if (pde->value == NULL_FRAME) {
			return (ERR_NO_MEMORY);
		}
		/* Always map page tables for users */
		pde->user = true;
		pde->present = true;
		pde->rw = true;
		pde->accessed = false;
		invlpg(pt);
		memset(pt, 0, PAGE_SIZE);
	}
	pte = pt->entries + get_pt_idx(va);

	if (pte->present)
	{
		/* If MMAP_REMAP, free old frame. Else, throw an error. */
		if (!(flags & MMAP_REMAP)) {
			return (ERR_ALREADY_MAPPED);
		}
		free_frame(pte->frame << 12u);
	}
	pte->value = pa;
	pte->present = true;
	pte->rw = (bool)(flags & MMAP_WRITE);
	pte->user = (bool)(flags & MMAP_USER);
	pte->accessed = false;
	pte->dirty = 0;
	invlpg(va);
	return (OK);
}

/*
** Maps the given virtual address to a random physical addresses.
*/
status_t
vmm_map_virtual(virtaddr_t va, mmap_flags_t flags)
{
	physaddr_t pa;
	status_t s;

	pa = alloc_frame();
	if (pa != NULL_FRAME)
	{
		s = vmm_map_physical(va, pa, flags);
		if (s == OK) {
#if KCONFIG_DEBUG_VMM
			/* Clean the new page with random shitty values */
			memset(va, 0x42, PAGE_SIZE);
#endif /* KCONFIG_DEBUG_VMM */
			return (OK);
		}
		free_frame(pa);
		return (s);
	}
	return (ERR_NO_MEMORY);
}

/*
** Unmaps a virtual address
*/
void
vmm_unmap(virtaddr_t va, munmap_flags_t flags)
{
	struct pagedir_entry *pde;
	struct pagetable_entry *pte;

	pde = get_pagedir()->entries + get_pd_idx(va);
	if (pde->present) {
		pte = get_pagetable(get_pd_idx(va))->entries + get_pt_idx(va);
		if (pte->present)
		{
			if (!(flags & MUNMAP_DONTFREE)) {
				free_frame(pte->frame << 12u);
			}
			pte->value = 0;
			invlpg(va);
		}
	}
}

/*
** Unmaps all user pagetables of the current virtual address space.
*/
void
vmm_unmap_pagetables(void)
{
	size_t i;
	struct page_dir *pd;

	pd = get_pagedir();
	for (i = 0; i < get_pd_idx(KERNEL_VIRTUAL_BASE); ++i)
	{
		if (pd->entries[i].present) {
			free_frame(pd->entries[i].frame << 12u);
			pd->entries[i].value = 0;
		}
	}
}

/*
** Copies the page tables of current page directory to the given one.
**
** Also sets resurive mapping for the given page directory.
*/
void
vmm_copy_pagetables(struct page_dir *pd)
{
	struct page_dir *cur_pd;
	size_t i;

	cur_pd = get_pagedir();
	for (i = get_pd_idx(KERNEL_VIRTUAL_BASE); i < 1024; ++i) {
		pd->entries[i] = cur_pd->entries[i];
	}
	pd->entries[1023].frame = vmm_get_frame(pd) >> 12u;
}

/*
** Inits the arch-dependant virtual memory manager, and then calls
** the arch-independant virtual memory manager init function.
*/
static void
arch_vmm_init(void)
{
	struct page_dir *pd;
	virtaddr_t pt;
	size_t i;

	/* Some assertions that can't be static_assert() */
	assert(IS_PAGE_ALIGNED(KERNEL_VIRTUAL_BASE));
	assert(IS_PAGE_ALIGNED(KERNEL_VIRTUAL_END));
	assert(IS_PAGE_ALIGNED(KERNEL_PHYSICAL_END));

	/* Remove mapping that is above the kernel */
	i = get_pt_idx(KERNEL_VIRTUAL_END);
	munmap(
		(uchar *)KERNEL_VIRTUAL_END + PAGE_SIZE,
		(1024 - i - 1) * PAGE_SIZE,
		MUNMAP_DONTFREE
	);

	/* Ensure everything is fine */
	assert(vmm_is_mapped(KERNEL_VIRTUAL_END));
	assert(!vmm_is_mapped((uchar *)KERNEL_VIRTUAL_END + PAGE_SIZE));
	assert(!vmm_is_mapped(get_virtaddr(get_pd_idx(KERNEL_VIRTUAL_END), 1023)));
	assert(!vmm_is_mapped(get_virtaddr(get_pd_idx(KERNEL_VIRTUAL_END) + 1, 0)));

	vmm_init();

	/* Allocate all page tables */
	pd = get_pagedir();
	for (i = get_pd_idx(KERNEL_VIRTUAL_BASE); i < 1024; ++i)
	{
		if (!pd->entries[i].present) {
			pt = kalloc_aligned(PAGE_SIZE);
			assert_ne(pt, NULL);
			memset(pt, 0, PAGE_SIZE);
			pd->entries[i].value = (uintptr)pt;
			pd->entries[i].present = true;
			pd->entries[i].rw = true;
			invlpg(get_pagetable(i));
		}
	}
}

NEW_INIT_HOOK(arch_vmm_init, &arch_vmm_init, INIT_LEVEL_VMM);
