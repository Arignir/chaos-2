/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/init.h>
#include <arch/x86/asm.h>
#include <drivers/vga.h>
#include <string.h>

/* Global definitions */
static uint const VGA_WIDTH = 80u;
static uint const VGA_HEIGHT = 25u;
static struct vga vga =
{
	.vga_attrib = 0,
	.vgabuff = (uint16 *)0xB8000,
	.cursor_x = 0,
	.cursor_y = 0,
};

/*
** Sets the current color for next outputs
*/
void
vga_set_color(enum vga_color bg, enum vga_color fg)
{
	vga.vga_attrib = ((bg << 4u) | (fg & 0x0F)) << 8u;
}

/*
** Moves the visual cursor to it's real coordinates.
*/
static void
move_cursor(void)
{
	uint tmp;

	tmp = vga.cursor_y * VGA_WIDTH + vga.cursor_x;
	outb(0x3D4, 14);
	outb(0x3D5, tmp >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, tmp);
}

/*
** Clears the screen
*/
void
vga_clear(void)
{
	uint i;
	uint16 blank;

	i = 0;
	blank = vga.vga_attrib | 0x20;
	while (i < VGA_WIDTH)
	{
		*(vga.vgabuff + i) = blank;
		++i;
	}
	i = 1;
	while (i < VGA_HEIGHT)
	{
		memcpy(vga.vgabuff + VGA_WIDTH * i, vga.vgabuff, sizeof(uint16) * VGA_WIDTH);
		++i;
	}
	vga.cursor_x = 0;
	vga.cursor_y = 0;
	move_cursor();
}

/*
** Scrolls the text
*/
static void
vga_scroll(void)
{
	uint16 blank;
	size_t i = 0;

	i = 0;
	blank = vga.vga_attrib | 0x20;
	memmove(vga.vgabuff, vga.vgabuff + VGA_WIDTH, (VGA_HEIGHT - 1) * VGA_WIDTH * sizeof(uint16));
	while (i < VGA_WIDTH)
	{
		*(vga.vgabuff + (VGA_HEIGHT - 1) * VGA_WIDTH + i) = blank;
		++i;
	}
}

/*
** Prints a single character on the screen.
** Doesn't move the cursor
*/
static int
vga_naked_putchar(int c)
{
	switch (c)
	{
	case '\n': /* new line */
		vga.cursor_x = 0;
		vga.cursor_y++;
		break;
	case '\t': /* tabulation */
		vga.cursor_x = (vga.cursor_x + 8u) & ~7u;
		break;
	case '\r': /* carriage return */
		vga.cursor_x = 0;
		break;
	case '\b': /* backspace */
		if (vga.cursor_x == 0) {
			vga.cursor_y -= (vga.cursor_y > 0);
			vga.cursor_x = VGA_WIDTH - 1;
		}
		else {
			vga.cursor_x -= (vga.cursor_x > 0);
		}
		break;
	default:
		*(vga.vgabuff + vga.cursor_y * VGA_WIDTH + vga.cursor_x) =
			vga.vga_attrib | (uchar)c;
		vga.cursor_x += 1;
		break;
	}
	vga.cursor_y += (vga.cursor_x >= VGA_WIDTH);
	vga.cursor_x *= (vga.cursor_x < VGA_WIDTH);
	while (vga.cursor_y >= VGA_HEIGHT) {
		vga_scroll();
		vga.cursor_y -= 1;
	}
	return (1);
}

/*
** Prints a character on the screen
*/
int
vga_putchar(int c)
{
	int ret;

	ret = vga_naked_putchar(c);
	move_cursor();
	return (ret);
}

/*
** Prints an array of characters on the screen
*/
int
vga_puts(char const *str)
{
	char const *s;

	s = str;
	while (*str) {
		vga_naked_putchar(*str);
		++str;
	}
	move_cursor();
	return (str - s);
}

/*
** Prints a known-size array of characters on the screen
*/
int
vga_putsn(char const *str, size_t n)
{
	while (n) {
		vga_naked_putchar(*str);
		++str;
		--n;
	}
	move_cursor();
	return (n);
}

/*
** Initializes the vga driver
** Called early by the arch_XXX_setup() function for
** debugging
*/
void
vga_init(void)
{
	vga_set_color(VGA_BLACK, VGA_WHITE);
	vga_clear();
}
