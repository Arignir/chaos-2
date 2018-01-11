;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  This file is part of the Chaos Kernel, and is made available under
;;  the terms of the GNU General Public License version 2.
;;
;;  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[global arch_jump_to_userspace]

%include "arch/x86/asm.inc"

;
; jump_to_userspace(void *stack, void (*main)(void));
;
; Jumps in userspace and calls the given function.
;
arch_jump_to_userspace:
	mov eax, [esp + 4]
	mov ebx, [esp + 8]

	mov ax, USER_DATA_SELECTOR | 0b11	; User data selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push USER_DATA_SELECTOR | 0b11		; Push user data sel
	push eax				; push user stack
	pushf
	push USER_CODE_SELECTOR | 0b11		; Push user code selector
	push ebx				; Push EIP (function to call)
	iret					; Let the magic happen
