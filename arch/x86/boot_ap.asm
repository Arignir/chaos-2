;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  This file is part of the Chaos Kernel, and is made available under
;;  the terms of the GNU General Public License version 2.
;;
;;  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "kconfig.inc"

%if KCONFIG_ENABLE_SMP

[global boot_ap]
[extern ap_setup]
[extern gdt]
[extern gdtptr]
[extern kthread_page_directory]
[extern ap_boot_stack]

%include "arch/x86/asm.inc"

[section .text]
align 16
lgdtptr:
	dw 4 * 8 - 1
	dd V2P(gdt)

[bits 16]
boot_ap:
	cli

	mov ax, 0xFFFF
	mov ds, ax

	lgdt [dword ds:V2P(lgdtptr) - 0xFFFF0]

	; Switch from real mode to protected mode
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	; Do a far jump to update code selector and eip
	jmp KERNEL_CODE_SELECTOR:dword V2P(.far_jump)

[bits 32]
.far_jump:

	; Set up protected-mode data segments
	mov ax, KERNEL_DATA_SELECTOR
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov ss, ax
	mov gs, ax

	; Load page directory
	mov eax, V2P(kthread_page_directory)
	mov cr3, eax

	; Enable paging
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	; Jump to high address space
	lea eax, [.higher_half]
	jmp eax
.higher_half:

	; Reload the GDT
	lgdt [gdtptr]

	; Switch to stack allocated by apic_start_ap()
	mov eax, [ap_boot_stack]
	mov esp, eax

	; Go to C code
	call ap_setup

.catch_fire:
	hlt
	jmp .catch_fire

%endif
