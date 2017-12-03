/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <arch/x86/interrupts.h>
#include <arch/x86/apic.h>

void
interrupts_handler(struct iframe *iframe)
{
	switch (iframe->int_num)
	{
	case INT_APIC_TIMER:
		apic_timer_ihandler(iframe);
		break;
	case INT_APIC_ERROR:
		apic_error_ihandler(iframe);
		break;
	case INT_APIC_SPURIOUS:
		apic_spurious_ihandler(iframe);
		break;
	default:
		panic("Received an unhandled exception (%#X)\n", iframe->int_num);
		break;
	}
}
