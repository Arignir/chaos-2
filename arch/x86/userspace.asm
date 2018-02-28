;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  This file is part of the Chaos Kernel, and is made available under
;;  the terms of the GNU General Public License version 2.
;;
;;  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[global arch_jump_to_userspace]

[bits 32]

%include "arch/x86/asm.inc"

;
; jump_to_userspace(void *stack, void (*main)(void));
;
; Jumps in userspace and calls the given function.
;
arch_jump_to_userspace:
	mov edx, [esp + 4]
	mov ecx, [esp + 8]

	mov esp, edx
	push dword 0x0				; Push NULL as the eip to return from main
	mov edx, esp

	mov ax, USER_DATA_SELECTOR | 0b11	; User data selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push dword USER_DATA_SELECTOR | 0b11	; Push user data sel
	push dword edx				; push user stack
	push dword 0x3202			; push eflags
	push dword USER_CODE_SELECTOR | 0b11	; Push user code selector
	push dword ecx				; Push EIP (function to call)
	iretd					; Let the magic happen
