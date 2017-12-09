/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

OUTPUT_FORMAT(elf32-i386);
ENTRY(start)

SECTIONS
{
	. = 1M;

	.text ALIGN(0x1000) : AT(ADDR(.text))
	{
		KEEP(*(.header))
		*(.text)
	}

	.rodata ALIGN(0x1000) : AT(ADDR(.rodata))
	{
		*(.rodata)
	}

	.data ALIGN(0x1000) : AT(ADDR(.data))
	{
		*(.data)
	}

	. = ALIGN(0x1000);
	PROVIDE(__start_bss = .);
	.bss ALIGN(0x1000) : AT(ADDR(.bss))
	{
		*(COMMON)
		*(.bss)
	}
	PROVIDE(__stop_bss = .);

	. = ALIGN(0x1000);

	__KERNEL_PHYSICAL_END = .;
}


