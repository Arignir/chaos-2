/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

.global start
.extern kmain

.section .header

/* Multiboot's magic number */
#define MULTIBOOT_MAGIC		0xe85250d6

/* Multiboot header - Must be at the very beginning of the binary */
.align 8
mb_header_start:
	.int MULTIBOOT_MAGIC			/* magic number (multiboot 2)			*/
	.int 0					/* architecture 0 (protected mode of i386)	*/
	.int mb_header_end - mb_header_start	/* header length				*/

	/* checksum */
	.int 0x100000000 - (0xe85250d6 + (mb_header_end - mb_header_start))

	/* required end tag */
	.short 0				/* type */
	.short 0				/* flags */
	.int 8					/* size */
mb_header_end:

.section .text

start:
	call kmain
