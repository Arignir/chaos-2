;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  This file is part of the Chaos Kernel, and is made available under
;;  the terms of the GNU General Public License version 2.
;;
;;  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[extern interrupts_handler]

%include "arch/x86/asm.inc"

;
; Labels to say if an interrupt handler have an error code
;
%define NO_ERROR_CODE		0
%define ERROR_CODE		1

;
; A macro that generates an interrupt handler,
; saving common registers and calling the real C handler.
;
; Args: vector, handler, error_code
;
%macro NEW_INTERRUPT_HANDLER 3
	[global %2]
	align 8
	%2:
%if %3 != ERROR_CODE
	push dword 0x0		; Push dummy error code
%endif
	push dword %1		; Push interrupt vector
	pusha			; Push edi, esi, ebp, esp, ebx, edx, ecx, eax

	push ds			; Save segment registers
	push es
	push fs
	push gs

	mov ax, KERNEL_DATA_SELECTOR
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push esp		; Push stack frame

	call interrupts_handler

	add esp, 0x4		; Pop stack frame

	pop gs
	pop fs
	pop es
	pop ds

	popa
	add esp, 0x8		; Clean up error code and interrupt number
	iret			; Return from interrupt (pop cs, eip, eflags, ss, esp)
%endmacro

[section .text]

; MACRO				ID	HANDLER					ERROR CODE
NEW_INTERRUPT_HANDLER		0x0,	x86_division_by_zero_ihandler,		NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x1,	x86_debug_ihandler,			NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x2,	x86_nmi_ihandler,			NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x3,	x86_breakpoint_ihandler,		NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x4,	x86_overflow_ihandler,			NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x5,	x86_out_of_bounds_ihandler,		NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x6,	x86_invalid_opcode_ihandler,		NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x7,	x86_device_na_ihandler,			NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x8,	x86_double_fault_ihandler,		ERROR_CODE
NEW_INTERRUPT_HANDLER		0x9,	x86_fpu_segment_overrun_ihandler,	NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0xA,	x86_invalid_tss_ihandler,		ERROR_CODE
NEW_INTERRUPT_HANDLER		0xB,	x86_segment_not_present_ihandler,	ERROR_CODE
NEW_INTERRUPT_HANDLER		0xC,	x86_stack_fault_ihandler,		ERROR_CODE
NEW_INTERRUPT_HANDLER		0xD,	x86_gp_fault_ihandler,			ERROR_CODE
NEW_INTERRUPT_HANDLER		0xE,	x86_page_fault_ihandler,		ERROR_CODE
NEW_INTERRUPT_HANDLER		0xF,	x86_unknown_int_ihandler,		NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x10,	x86_fpu_exception_ihandler,		NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x11,	x86_alignment_check_ihandler,		ERROR_CODE
NEW_INTERRUPT_HANDLER		0x12,	x86_machine_check_ihandler,		NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x13,	x86_simd_fd_exception_ihandler,		NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x14,	x86_virt_exception_ihandler,		NO_ERROR_CODE

NEW_INTERRUPT_HANDLER		0x20,	x86_apic_timer_ihandler,		NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0x33,	x86_apic_error_ihandler,		NO_ERROR_CODE
NEW_INTERRUPT_HANDLER		0xFF,	x86_apic_spurious_ihandler,		NO_ERROR_CODE

NEW_INTERRUPT_HANDLER		0xFE,	x86_unknown_ihandler,			NO_ERROR_CODE

