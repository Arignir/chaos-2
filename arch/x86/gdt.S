/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

.global gdtptr

.section .rodata
.align 16
gdtptr:
	.short gdt_end - gdt_start - 1
	.int gdt

.section .data
.align 16
gdt:
gdt_start:
	/* NULL entry */
	.int 0
	.int 0

	/* Kernel Code Selector */
	.short 0xFFFF		/* limit 15:0	*/
	.short 0x0000		/* base 15:0	*/
	.byte 0x00		/* base 23:16	*/
	.byte 0b10011010	/* P(1) DPL(00) (1) C(1) D(0) R(1) A(0) */
	.byte 0b11001111	/* G(1) S(1) (0) (0) limit 19:16 */
	.byte 0x00		/* base 31:24 */

	/* Kernel Data Selector */
	.short 0xFFFF		/* limit 15:0	*/
	.short 0x0000		/* base 15:0	*/
	.byte 0x00		/* base 23:16	*/
	.byte 0b10010010	/* P(1) DPL(00) (1) C(0) E(0) W(1) A(0) */
	.byte 0b11001111	/* G(1) S(1) (0) (0) limit 19:16 */
	.byte 0x00		/* base 31:24 */

	/* NULL entry */
	.int 0
	.int 0
gdt_end:
