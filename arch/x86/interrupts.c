/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <arch/x86/interrupts.h>
#include <arch/x86/asm.h>

static ihandler_t irqhandlers[X86_INT_MAX];

/*
**
** Implementation of arch-independant functions defined in <kernel/interrupts.h>
**
*/

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
push_interrupt_state(void *save)
{
	*(bool *)save = get_eflags().IF;
}

/*
** Restores the current interrupt state
*/
void
pop_interrupt_state(void *save)
{
	union eflags e;

	e = get_eflags();
	e.IF = *(bool *)save;
	set_eflags(e.value);
}

/*
** Registers a handler for the given interrupt vector
*/
void	register_int_handler(uint vector, void *handler)
{
	assert(vector < X86_INT_NB);
	irqhandlers[vector] = handler;
}

/*
** UnRegisters the handler for the given interrupt vector
*/
void	unregister_int_handler(uint vector)
{
	assert(vector < X86_INT_NB);
	irqhandlers[vector] = NULL;
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
