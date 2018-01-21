/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/thread.h>
#include <kernel/vaspace.h>
#include <arch/x86/vmm.h>

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
