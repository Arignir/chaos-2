;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  This file is part of the Chaos Kernel, and is made available under
;;  the terms of the GNU General Public License version 2.
;;
;;  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%ifndef _ARCH_X86_ASM_INC
%define _ARCH_X86_ASM_INC

%define NULL_SELECTOR			0x00
%define KERNEL_CODE_SELECTOR		0x08
%define KERNEL_DATA_SELECTOR		0x10

; These values must match the ones in arch/x86/linker.ld
%define KERNEL_VIRTUAL_BASE		(0xC0000000)
%define KERNEL_PAGE_INDEX		(KERNEL_VIRTUAL_BASE >> 22)

%define P2V(x)				((x) + KERNEL_VIRTUAL_BASE)
%define V2P(x)				((x) - KERNEL_VIRTUAL_BASE)

%endif
