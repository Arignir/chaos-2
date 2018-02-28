/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Louis Guittard <louis.guittard@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <chaosdef.h>
#include <kernel/init.h>
#include <kernel/interrupts.h>
#include <kernel/mutex.h>
#include <arch/x86/apic.h>
#include <arch/x86/asm.h>
#include <arch/x86/interrupts.h>
#include <drivers/keyboard.h>
#include <stdio.h>

static char const fr_azerty_charset[128] =
{
	0,
	[9] = '_',
	[16] = 'a',
	[17] = 'z',
	[18] = 'e',
	[19] = 'r',
	[20] = 't',
	[21] = 'y',
	[22] = 'u',
	[23] = 'i',
	[24] = 'o',
	[25] = 'p',
	[30] = 'q',
	[31] = 's',
	[32] = 'd',
	[33] = 'f',
	[34] = 'g',
	[35] = 'h',
	[36] = 'j',
	[37] = 'k',
	[38] = 'l',
	[39] = 'm',
	[44] = 'w',
	[45] = 'x',
	[46] = 'c',
	[47] = 'v',
	[48] = 'b',
	[49] = 'n',
	[28] = '\n',
	[14] = '\b',
	[52] = '/',
	[57] = ' ',
};

static char input_buffer[PAGE_SIZE] = { 0 };
static size_t input_write_idx = 0;
static size_t input_read_idx = 0;
static struct mutex keyboard_lock = MUTEX_DEFAULT;

static int leds_state = 0;

static int caps_lock = false;
static int left_shift = false;
static int right_shift = false;

static bool
keyboard_write_command(uchar cmd)
{
	int retries = KEYBOARD_MAX_RETRIES;

	while ((inb(PORT_PS2_KEYBOARD_STATUS) & KEYBOARD_STATUS_WRNOTOK) && --retries)
		pause();
	if (retries > 0)
		outb(PORT_PS2_KEYBOARD_CMD, cmd);
	return (retries <= 0);
}

static bool
keyboard_write_data(uchar data)
{
	int retries = KEYBOARD_MAX_RETRIES;

	while ((inb(PORT_PS2_KEYBOARD_STATUS) & KEYBOARD_STATUS_WRNOTOK) && --retries)
		pause();
	if (retries > 0)
		outb(PORT_PS2_KEYBOARD_DATA, data);
	return (retries <= 0);
}

__used
static uchar
keyboard_read_data(void)
{
	int retries = KEYBOARD_MAX_RETRIES;

	while ((inb(PORT_PS2_KEYBOARD_STATUS) & KEYBOARD_STATUS_RDOK) && --retries)
		pause();
	return (retries > 0 ? inb(PORT_PS2_KEYBOARD_DATA) : -1);
}

static void
keyboard_ihandler(void)
{
	unsigned char code;
	size_t check_overwrite;
	int is_pressed;

	apic_eoi();
	mutex_acquire(&keyboard_lock);
	code = inb(PORT_PS2_KEYBOARD_DATA);
	is_pressed = !(code & 0x80);
	if (is_pressed)
	{
		switch (code)
		{
		case (SC1_CAPSLOCK_PRESSED):
			caps_lock = !caps_lock;
			leds_state ^= 4;

			/* This doesn't seem to work - TODO Investigate */
			keyboard_write_command(KEYBOARD_CMD_LED);
			keyboard_write_data(leds_state);
			break;
		case (SC1_LSHIFT_PRESSED):
			left_shift = true;
			break;
		case (SC1_RSHIFT_PRESSED):
			right_shift = true;
			break;
		default:
			break;
		}
		code = fr_azerty_charset[(int)code];
		check_overwrite = (input_write_idx + 1) % PAGE_SIZE;
		if (check_overwrite != input_read_idx && code != '\0')
		{
			if (code >= 'a' && code <= 'z') {
				code -= 32 * (caps_lock ^ (left_shift | right_shift));
			}
			input_buffer[input_write_idx] = code;
			input_write_idx = check_overwrite;
		}
	}
	else
	{
		switch (code)
		{
		case (SC1_LSHIFT_RELEASED):
			left_shift = false;
			break;
		case (SC1_RSHIFT_RELEASED):
			right_shift = false;
			break;
		default:
			break;
		}
	}
	mutex_release(&keyboard_lock);
}

/*
** Sends the next char or loops until the user presses a key.
*/
char
keyboard_next_input(void)
{
	char c;
	int_state_t state;

	push_interrupts_state(&state);
	enable_interrupts();
	while (1)
	{
		mutex_acquire(&keyboard_lock);
		if (input_read_idx == input_write_idx)
			mutex_release(&keyboard_lock);
		else
			break;
		yield();
	}
	c = input_buffer[input_read_idx];
	input_read_idx = (input_read_idx + 1) % PAGE_SIZE;
	mutex_release(&keyboard_lock);
	pop_interrupts_state(&state);
	return (c);
}

static void
keyboard_init(void)
{
	register_int_handler(INT_KEYBOARD, &keyboard_ihandler);

	/* This doesn't seem to work - TODO Investigate */
	assert(!(keyboard_write_command(KEYBOARD_CMD_SCANCODE)));
	assert(!(keyboard_write_data(KEYBOARD_SCANCODE_SET_2)));
	printf("Keyboard set to Scancode 2\n");
}

NEW_INIT_HOOK(keyboard_init, &keyboard_init, INIT_LEVEL_DRIVERS);
