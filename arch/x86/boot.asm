;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  This file is part of the Chaos Kernel, and is made available under
;;  the terms of the GNU General Public License version 2.
;;
;;  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[global start]
[global detect_cpuid]
[extern kmain]
[extern gdtptr]
[extern __start_bss]
[extern __stop_bss]
[extern mb_tag]

%include "arch/x86/asm.inc"
%include "multiboot.inc"

; Multiboot header - Must be at the very beginning of the binary
[section .header]
align 8
mb_header_start:
	dd MULTIBOOT2_HEADER_MAGIC		; magic number
	dd 0					; protected mode
	dd mb_header_end - mb_header_start	; header length

	; checksum
	dd 0x100000000 - (0xe85250d6 + (mb_header_end - mb_header_start))

	; required end tag
	dw MULTIBOOT_HEADER_TAG_END	; type
	dw 0				; flags
	dd 8				; size
mb_header_end:

[section .text]
start:
	; Set up boot stack
	mov esp, boot_stack_top

	; Check we have been booted using multiboot
	cmp eax, MULTIBOOT2_BOOTLOADER_MAGIC
	jne 0				; Crash if it's not the case

	; Save ebx value now in a scratch register, so that we can use it after bss's reset
	add ebx, 8
	mov ecx, ebx

	; Load the new GDT
	lgdt [gdtptr]

	; Load all data segment selectors
	mov ax, KERNEL_DATA_SELECTOR
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; Do a far jump to update code selector
	jmp KERNEL_CODE_SELECTOR:.far_jump

.far_jump:

	; Clear BSS section
	mov eax, __start_bss
	.loop:
	cmp eax, __stop_bss
	je .endloop
	mov byte [eax], 0
	inc eax
	jmp .loop
	.endloop:

	; TODO This will need to be updated when virtual memory will be available
	; Save multiboot's structure
	mov edx, mb_tag
	mov dword [edx], ecx

	; Call kernel main and common entry point
	call kmain

.catch_fire:
	hlt				; And catch fire
	jmp .catch_fire

;
; Detects for the CPUID instruction.
; Returns false if it's available, true otherwise.
;
detect_cpuid:
	pushfd				; Save EFLAGS
	pushfd				; Store EFLAGS
	xor dword [esp], 0x00200000	; Invert the ID bit in stored EFLAGS
	popfd				; Load stored EFLAGS (with ID bit inverted)
	pushfd				; Store EFLAGS again (ID bit may or may not be inverted)
	pop eax				; eax = modified EFLAGS (ID bit may or may not be inverted)
	xor eax, dword [esp]		; eax = whichever bits were changed
	popfd				; Restore original EFLAGS
	and eax, 0x00200000		; eax = zero if ID bit can't be changed, else non-zero
	ret

[section .bss]
align 4096
boot_stack_bottom:
	resb 4096 * 16
boot_stack_top:
