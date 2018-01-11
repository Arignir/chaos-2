/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_TSS_H_
# define _ARCH_X86_TSS_H_

# include <chaosdef.h>

/*
** The Task State Segment.
**
** As we're doing software multitasking, most of the TSS is useless.
*/
struct tss
{
	uint16 previous;
	uint16 _reserved_0;
	uint32 esp0;
	uint16 ss0;
	uint16 _reserved_1;
	uint32 esp1;
	uint16 ss1;
	uint16 _reserved_2;
	uint32 esp2;
	uint16 ss2;
	uint16 _reserved_3;
	uint32 cr3;
	uint32 eip;
	uint32 eflags;
	uint32 eax;
	uint32 ecx;
	uint32 edx;
	uint32 ebx;
	uint32 esp;
	uint32 ebp;
	uint32 esi;
	uint32 edi;
	uint16 es;
	uint16 _reserved_4;
	uint16 cs;
	uint16 _reserved_5;
	uint16 ss;
	uint16 _reserved_6;
	uint16 ds;
	uint16 _reserved_7;
	uint16 fs;
	uint16 _reserved_8;
	uint16 gs;
	uint16 _reserved_9;
	uint16 ldt_ss;
	uint16 _reserved_10;
	uint16 _reserved_11;
	uint16 base_addr;
};

static_assert(sizeof(struct tss) == 26 * sizeof(uintptr));

struct gdt_tss_entry
{
	uint limit_low		: 16;
	uint base_low		: 24;

	uint _one		: 1;
	uint busy		: 1;
	uint type		: 3;
	uint dpl		: 2;
	uint present		: 1;

	uint limit_high		: 4;
	uint available		: 1;
	uint _zero		: 2;
	uint granularity	: 1;
	uint base_high		: 8;
} __packed;

static_assert(sizeof(struct gdt_tss_entry) == 2 * sizeof(uintptr));

void	tss_setup(void);

#endif /* !_ARCH_X86_TSS_H_ */
