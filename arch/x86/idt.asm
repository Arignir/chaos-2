;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  This file is part of the Chaos Kernel, and is made available under
;;  the terms of the GNU General Public License version 2.
;;
;;  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[global idt_setup]
[global idt_load]
[extern interrupts_handler]

%include "arch/x86/asm.inc"

[section .text]

; Create the 256 interrupt handlers
isr:
%assign i 0x0
%rep 256
	; if it has an error code
	%if i == 8 || (i >= 10 && i <= 14) || i == 17
		nop	; Two nops have the same length as a pushl
		nop
		push dword i
		jmp interrupt_common
	%else
		push dword 0	; Fake error code
		push dword i
		jmp interrupt_common
	%endif
	%assign i (i + 1)
%endrep

interrupt_common:
	pushad			; Push edi, esi, ebp, esp, ebx, edx, ecx, eax

	push ds			; Save segment registers
	push es
	push fs
	push gs

	mov ax, KERNEL_DATA_SELECTOR
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push esp		; Push iframe

	call interrupts_handler

	add esp, 0x4		; Pop iframe

	pop gs
	pop fs
	pop es
	pop ds

	popad
	add esp, 0x8		; Clean up error code and interrupt number
	iretd			; Return from interrupt (pop cs, eip, eflags, ss, esp)

[section .rodata]

;
; IDT descriptor structure
;
idtptr:
	dw idt_end - idt_start - 1
	dd idt

[section .data]

;
; Interrupt Descriptor Table
;
idt:
idt_start:
	%rep 256
		dw 0			; Low 16 bits of ISR
		dw KERNEL_CODE_SELECTOR	; Code selector
		db 0
		db 0x8E			; Present, ring-0, 32-bits interrupt gate
		dw 0			; High 16 bits of ISR
	%endrep
idt_end:

[section .text]

;
; Sets up the IDT
;
idt_setup:
	push ebx
	push esi
	push edi
	push ecx

	mov esi, isr
	mov edi, idt
	mov ecx, 256

.loop:
	mov ebx, esi
	mov word [edi], bx
	shr ebx, 16
	mov word [edi + 6], bx

	add esi, 9 ; Size of an isr (pushb + pushb + jmp) - See above
	add edi, 8

	loop .loop

	; Syscall interrupt needs to be set as a trap gate with DPL=3
	mov edi, idt
	add edi, 0x80 * 8 + 5	; edi points to the syscall entry
	mov byte [edi], 0xEF	; Present, DPL=3, Trap gate

	pop ecx
	pop edi
	pop esi
	pop ebx
	ret

;
; Loads the IDT
;
idt_load:
	lidt [idtptr]
	ret
