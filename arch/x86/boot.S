/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

.global start
.global detect_cpuid
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

/*
** Detects for the CPUID instruction.
** Returns false if it's available, true otherwise.
*/
detect_cpuid:
	pushfl				/* Save EFLAGS */
	pushfl				/* Store EFLAGS */
	xorl $0x00200000, (%esp)	/* Invert the ID bit in stored EFLAGS */
	popfl				/* Load stored EFLAGS (with ID bit inverted) */
	pushfl				/* Store EFLAGS again (ID bit may or may not be inverted) */
	popl %eax			/* eax = modified EFLAGS (ID bit may or may not be inverted) */
	xorl (%esp), %eax		/* eax = whichever bits were changed */
	popfl				/* Restore original EFLAGS */
	andl $0x00200000, %eax		/* eax = zero if ID bit can't be changed, else non-zero */
	ret

.section .bss
.align 4096
boot_stack_bottom:
	.space 4096 * 16
boot_stack_top:
