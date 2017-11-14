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
.extern gdtptr

#include "arch/x86/x86.h"
#include "kernel/multiboot2.h"

/* Multiboot header - Must be at the very beginning of the binary */
.section .header
.align 8
mb_header_start:
	.int MULTIBOOT2_HEADER_MAGIC		/* magic number (multiboot 2)			*/
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
	/* Set up boot stack */
	mov $boot_stack_top, %esp

	/*
	** We store kmain parameters now, in case these registers get modified afterward
	** eax should contain MULTIBOOT2_BOOTLOADER_MAGIC if we have been booted via a
	** multiboot compliant bootloader. In that case, ebx contains the address of the
	** multiboot structure.
	*/
	push %ebx
	push %eax

	/* Load the new GDT */
	mov $gdtptr, %eax
	lgdt (%eax)

	/* Load all data segment selectors */
	movw $KERNEL_DATA_SELECTOR, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	/* Do a far jump to update code selector */
	pushl $KERNEL_CODE_SELECTOR
	pushl $.far_jump
	lret

.far_jump:
	call kmain
.catch_fire:
	hlt				/* And catch fire */
	jmp .catch_fire

.section .bss
.align 4096
boot_stack_bottom:
	.space 4096 * 16
boot_stack_top:
