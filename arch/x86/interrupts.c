/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/syscall.h>
#include <kernel/memory.h>
#include <kernel/vmm.h>
#include <kernel/thread.h>
#include <arch/x86/interrupts.h>
#include <arch/x86/ioapic.h>
#include <arch/x86/asm.h>
#include <stdio.h>

static ihandler_t irqhandlers[X86_INT_NB];

/*
**
** Implementation of arch-independant functions defined in <kernel/interrupts.h>
**
*/

/*
** Tests if interrupts are enabled
*/
bool
are_interrupts_enable(void)
{
	return (get_eflags().IF);
}

/*
** Enables interrupts
*/
void
enable_interrupts(void)
{
	sti();
}

/*
** Disables interrupts
*/
void
disable_interrupts(void)
{
	cli();
}

/*
** Stores in 'save' the current interrupt state
*/
void
push_interrupts_state(int_state_t *save)
{
	*save = get_eflags().IF;
}

/*
** Restores the current interrupt state
*/
void
pop_interrupts_state(int_state_t const *save)
{
	union eflags e;

	e = get_eflags();
	e.IF = (bool)*save;
	set_eflags(e.value);
}

/*
** Registers a handler for the given interrupt vector
*/
void
register_int_handler(uint vector, void const *handler)
{
	assert(vector < X86_INT_NB);
	irqhandlers[vector] = handler;
	ioapic_unmask_int(vector);
}

/*
** UnRegisters the handler for the given interrupt vector
*/
void
unregister_int_handler(uint vector)
{
	assert(vector < X86_INT_NB);
	irqhandlers[vector] = NULL;
}

void
halt()
{
	hlt();
}

/*
** Exceptions handler
*/

/*
** Page fault handler.
**
** Kills current thread.
*/
static void
pagefault_handler(struct iframe *iframe)
{
	virtaddr_t addr;
	virtaddr_t align_addr;

	addr = (virtaddr_t)get_cr2();
	align_addr = (virtaddr_t)ROUND_DOWN(get_cr2(), PAGE_SIZE);
	/* Page faults in kernel space are lethals */
	if (vmm_is_mapped_in_kernelspace(align_addr)) {
		panic("Page Fault in kernel space at address %#p.\n"
			"\teip: %#p\n"
			"\tStack: %#p\n"
			"\tPresent: %y\n"
			"\tWrite: %y\n"
			"\tUser-mode: %y\n"
			"\tInstruction fetch: %y\n",
			(void *)addr,
			iframe->eip,
			iframe->esp,
			iframe->err_code & 0b00001,
			iframe->err_code & 0b00010,
			iframe->err_code & 0b00100,
			iframe->err_code & 0b10000
		);
	}
	printf("Segmentation Fault (EIP: %#p)\n", addr);
	thread_exit(EXIT_PAGEFAULT);
}

/*
** Handles the syscall interruption.
**
** Arguments are in edi, esi, edx and ecx.
** Syscall id is in eax.
** Return value must be placed in eax.
*/
void
syscall_handler(struct iframe *iframe)
{
	switch (iframe->eax)
	{
	case SYSCALL_CLONE:
		iframe->eax = sys_clone(
			(void (*)(void))iframe->edi
		);
		break;
	case SYSCALL_EXIT:
		sys_exit(iframe->edi);
		break;
	case SYSCALL_EXEC:
		iframe->eax = sys_exec((char const *)iframe->edi);
		break;
	case SYSCALL_OPEN:
		iframe->eax = sys_open(
			(char const *)iframe->edi
		);
		break;
	case SYSCALL_READ:
		iframe->eax = sys_read(
			(int)iframe->edi,
			(char *)iframe->esi,
			(size_t)iframe->edx
		);
		break;
	case SYSCALL_WRITE:
		iframe->eax = sys_write(
			(int)iframe->edi,
			(char const *)iframe->esi,
			(size_t)iframe->edx
		);
		break;
	case SYSCALL_CLOSE:
		iframe->eax = sys_close(
			(file_handle_t)iframe->edi
		);
		break;
	case SYSCALL_OPENDIR:
		iframe->eax = sys_opendir(
			(char const *)iframe->edi
		);
		break;
	case SYSCALL_READDIR:
		iframe->eax = sys_readdir(
			(dir_handle_t)iframe->edi,
			(struct dirent *)iframe->esi
		);
		break;
	case SYSCALL_CLOSEDIR:
		iframe->eax = sys_closedir(
			(dir_handle_t)iframe->edi
		);
		break;
	case SYSCALL_KEYBOARD:
		iframe->eax = sys_keyboard();
		break;
	default:
		panic("Unknown syscall\n");
		break;
	}
}

/*
** Interrupt handler routine
*/
void
interrupts_handler(struct iframe *iframe)
{
	switch (iframe->int_num)
	{
	/* Interrupt Request */
	case INT_PAGE_FAULT:
		pagefault_handler(iframe);
		break;
	case INT_IRQ0...X86_INT_NB:
		if (irqhandlers[iframe->int_num]) {
			irqhandlers[iframe->int_num](iframe);
		} else {
			panic("Unhandled interrupt request (%#X)\n", iframe->int_num);
		}
		break;
	default:
		panic("Unhandled interrupt (%#X)\n", iframe->int_num);
		break;
	}
}
