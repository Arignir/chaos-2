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

/* Multiboot's magic number */
#define MULTIBOOT_MAGIC		0xe85250d6

/* Multiboot header - Must be at the very beginning of the binary */
.section .header
.align 8
mb_header_start:
	.int MULTIBOOT_MAGIC			/* magic number (multiboot 2)			*/
	.int 0					/* architecture 0 (protected mode of i386)	*/
	.int mb_header_end - mb_header_start	/* header length				*/

	/* checksum */
	.int 0x100000000 - (0xe85250d6 + (mb_header_end - mb_header_start))

	/* required end tag */
	.short 0			/* type */
	.short 0			/* flags */
	.int 8				/* size */
mb_header_end:

.section .text
start:
	mov boot_stack_top, %esp	/* Set boot stack */
	call kmain
.catch_fire:
	hlt				/* And catch fire */
	jmp .catch_fire

.section .bss
.align 4096
boot_stack_bottom:
	.space 4096 * 16
boot_stack_top:
