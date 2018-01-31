/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/vaspace.h>
#include <kernel/pmm.h>
#include <kernel/kalloc.h>
#include <arch/x86/vmm.h>
#include <string.h>

extern struct pagedirectory *kthread_page_directory[];

/*
** Switches to the given virtual address space.
*/
void
arch_vaspace_switch(struct vaspace *new)
{
	if (vmm_get_frame(get_pagedir()) != new->arch.pagedir) {
		set_cr3(new->arch.pagedir);
	}
}

/*
** Switches to a new page directory and frees the arch-dependant part of the
** old vaspace.
**
** For x86, this includes the page tables and the page directory.
*/
void
arch_vaspace_free(void)
{
	struct vaspace *vaspace;

	vaspace = current_vaspace();
	vmm_unmap_pagetables();
	set_cr3(V2P(kthread_page_directory));
	kfree(vaspace->arch.pagedir_virt);
}

/*
** Creates a new virtual address space and returns it, or NULL if an error occured.
**
** This must be called using the kernel's page directory.
*/
struct vaspace *
arch_new_vaspace(void)
{
	struct vaspace *vaspace;
	struct page_dir *pd;

	vaspace = kalloc(sizeof(*vaspace));
	pd = kalloc_aligned(sizeof(*pd));
	if (!vaspace || !pd) {
		kfree(vaspace);
		kfree(pd);
		return (NULL);
	}
	vaspace_init(vaspace);
	vaspace->arch.pagedir = vmm_get_frame(pd);
	vaspace->arch.pagedir_virt = pd;
	vmm_copy_pagetables(pd);
	return (vaspace);
}
