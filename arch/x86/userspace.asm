;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  This file is part of the Chaos Kernel, and is made available under
;;  the terms of the GNU General Public License version 2.
;;
;;  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[global jump_to_userspace]

;
; jump_to_userspace(void *stack, void (*main)(void));
;
; Jumps in userspace and calls the given function.
;
; TODO FIXME Currently, this function doens't go to userspace and stays in ring0.
;
jump_to_userspace:
	mov eax, [esp + 4]
	mov ebx, [esp + 8]
	mov esp, eax
	call ebx
