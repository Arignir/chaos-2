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

struct scheduler_stack {
	uintptr edi;
	uintptr esi;
	uintptr ebp;
	uintptr esp;
	uintptr ebx;
	uintptr edx;
	uintptr ecx;
	uintptr eax;
	uintptr eflags;
	uintptr eip;
	uintptr fake_ret;
	uintptr arg1;
	uintptr arg2;
	uintptr __required;
} __aligned;

/*
** Finishes setting up the new thread, by preparing the stack for the scheduler.
**
** The thread must be locked.
*/
void
arch_thread_clone(struct thread *t)
{
	/*
	** Setup a quick stack frame for the return of 'scheduler' in 'enter_scheduler'
	*/
	struct scheduler_stack default_stack = {
		.eax = 0x0,
		.ecx = 0x0,
		.edx = 0x0,
		.ebx = 0x0,
		.esp = 0x0,
		.ebp = 0x0,
		.esi = 0x0,
		.edi = 0x0,
		.eflags = 0x3202,
		.eip = (uintptr)&arch_jump_to_userspace,
		.fake_ret = 0x0,
		.arg1 = (uintptr)t->stack_top,
		.arg2 = (uintptr)t->entry,
		.__required = 0x0,	/* Required by Intel, or ret will throw */
	};

	/* If it's a kernel thread, set DPL accordingly. */
	if (!t->user) {
		default_stack.eflags = 0x3002;
		default_stack.eip = (uintptr)t->entry;
		default_stack.arg1 = 0x0;
		default_stack.arg2 = 0x0;
	}

	t->stack_saved = (uchar *)t->stack_saved - sizeof(default_stack);
	*(struct scheduler_stack *)t->stack_saved = default_stack;
}

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
