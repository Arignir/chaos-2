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
[global kthread_page_directory]
[global bsp_kernel_stack_top]
[global bsp_kernel_stack_bottom]
[extern kmain]
[extern gdtptr]
[extern gdtptr_phys]
[extern __start_bss]
[extern __stop_bss]
[extern mb_tag]

%include "arch/x86/asm.inc"
%include "multiboot.inc"
%include "kconfig.inc"

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

;
; Kernel start point.
;
; The bootloader drops us here following the multiboot's specification.
; The eax register should contain the multiboot2 bootloader's magic value, and
; 	we should crash if it's not the case.
; The ebx register should contain the multiboot2 structure, and should therefor
; 	NOT be modified until the bss section is cleaned up
[section .text]
start:
	; Set up boot stack
	mov esp, V2P(bsp_kernel_stack_top)

	; Check we have been booted using multiboot
	cmp eax, MULTIBOOT2_BOOTLOADER_MAGIC
	jne 0				; Crash if it's not the case

	; Load the new GDT
	mov eax, V2P(gdtptr_phys)
	lgdt [eax]

	; Load all data segment selectors
	mov ax, KERNEL_DATA_SELECTOR
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; Do a far jump to update code selector
	jmp KERNEL_CODE_SELECTOR:V2P(.far_jump)

.far_jump:

	; Set-up recursive mapping
	mov eax, V2P(kthread_page_directory)
	or eax, 0x3		; Present + Writtable
	mov dword [V2P(kthread_page_directory.last_entry)], eax

	; Map the kernel page table
	mov eax, V2P(kthread_kernel_pagetable)
	or eax, 0x3		; Present + Writtable
	mov dword [V2P(kthread_page_directory.kernel_entry)], eax

	mov eax, 0
	mov edx, 0
	mov edi, V2P(kthread_kernel_pagetable)
.fill_page:			; Fill the page table containing the kernel
	mov ecx, edx
	or ecx, 0x3		; Present + Writtable
	mov [edi + eax * 4], ecx
	add edx, 4096
	inc eax
	cmp eax, 1024
	jne .fill_page		; Works because jmp are relative, no need to use V2P().

	; Load page directory
	mov eax, V2P(kthread_page_directory)
	mov cr3, eax

	; Enable 4MiB pages (only to identity map the kernel)
	mov eax, cr4
	or eax, 0b10000
	mov cr4, eax

	; Enable paging and protected mode
	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax

	; Jump to high address space
	lea eax, [.higher_half]
	jmp eax
.higher_half:

	; Remove the 4MiB entry of the page directory
	mov dword [kthread_page_directory.first_entry], 0x0

	; Flush tlb
	mov eax, cr3
	mov cr3, eax

	; Disable 4MiB pages
	mov eax, cr4
	and eax, ~0b10000
	mov cr4, eax

	; Reload the GDT
	lgdt [gdtptr]

	; Reset boot stack
	mov esp, bsp_kernel_stack_top

	; Clear BSS section
	mov eax, __start_bss
	.loop:
	cmp eax, __stop_bss
	je .endloop
	mov byte [eax], 0
	inc eax
	jmp .loop
	.endloop:

	; Save multiboot's structure
	mov edx, mb_tag
	add ebx, 8
	add ebx, KERNEL_VIRTUAL_BASE
	mov dword [edx], ebx

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
bsp_kernel_stack_bottom:
	resb 4096 * KCONFIG_KERNEL_STACK_SIZE
bsp_kernel_stack_top:

[section .data]
align 4096
kthread_page_directory:
	.first_entry:
	dd 0x00000083			; Identity map the first entry to avoid crash
	times (KERNEL_PAGE_INDEX - 1) dd 0
	.kernel_entry:			; Used at a later point.
	dd 0
	times (1024 - KERNEL_PAGE_INDEX - 2) dd 0
	.last_entry:			; Used for recursive mapping
	dd 0

kthread_kernel_pagetable:
	times 1024 dd 0

section .bss

