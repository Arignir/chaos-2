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

%include "arch/x86/asm.inc"

[section .text]
align 16
lgdtptr:
	dw 4 * 8 - 1
	dd gdt

[bits 16]
boot_ap:
	cli

	mov ax, 0xFFFF
	mov ds, ax

	lgdt [dword ds:lgdtptr - 0xFFFF0]

	; Switch from real mode to protected mode
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	; Do a far jump to update code selector and eip
	jmp KERNEL_CODE_SELECTOR:dword .far_jump

[bits 32]
.far_jump:
	; Set up protected-mode data segments
	mov ax, KERNEL_DATA_SELECTOR
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov ss, ax
	mov gs, ax

	; Switch to stack allocated by mp_start_aps()
	;//mov (boot_ap-4), %esp
	mov esp, boot2_stack_top

	call ap_setup

.catch_fire:
	hlt
	jmp .catch_fire

[section .bss]
align 4096
boot2_stack_bottom:
	resb 4096 * 16
boot2_stack_top:

%endif
