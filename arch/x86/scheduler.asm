;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  This file is part of the Chaos Kernel, and is made available under
;;  the terms of the GNU General Public License version 2.
;;
;;  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[global enter_scheduler]
[global enter_scheduler_at]
[extern reschedule]

[bits 32]

%include "arch/x86/asm.inc"

;
; void enter_scheduler(void *scheduler_stack)
;
; Pushes the content of registers on stack, saves the current stack pointer, switches
; to the given scheduler stack and enters the scheduler.
;
; When the scheduler returns (from an other thread switching to this one), switches to the
; new, freshly returned stack, restores registers and returns.
;
; _WARNING_:
; Interrupts must be disabled before calling this function.
; The current thread must be locked.
;
enter_scheduler:
	mov eax, [esp + 4]	; Put in eax 'scheduler_stack'

	pushfd
	pushad			; TODO Optimize this (we don't need to push everything)

	mov ecx, esp		; Save thread's stack
	mov esp, eax		; Switch to scheduler stack

	push ecx		; Push thread's stack as parameter to reschedule

	call reschedule

	pop ecx

	mov esp, eax

	popad
	popfd
	ret

;
; __noreturn void enter_scheduler_at(void *scheduler_stack, void (*func_ptr)(void))
;
; Switches to the given scheduler stack and calls the given function pointer.
; This function never returns, and so must do the given function pointer.
;
; _WARNING_:
; Interrupts must be disabled before calling this function.
; The current thread must be locked.
;
enter_scheduler_at:
	mov eax, [esp + 8]
	mov esp, [esp + 4]	; Switch to scheduler stack

	call eax		; Call given function pointer

	cli			; Should never reach this
	hlt
