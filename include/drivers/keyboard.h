/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Louis Guittard <louis.guittard@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _DRIVER_KEYBOARD_H_
# define _DRIVER_KEYBOARD_H_

# define KEYBOARD_MAX_RETRIES		10000
# define SC1_CAPSLOCK_PRESSED		0x3A
# define SC1_LSHIFT_PRESSED		0x2A
# define SC1_RSHIFT_PRESSED		0x36
# define SC1_LSHIFT_RELEASED            0xAA
# define SC1_RSHIFT_RELEASED            0xB6

# define KEYBOARD_ACK			0xFA
# define KEYBOARD_RESEND		0xFE

# define KEYBOARD_CMD_SCANCODE		0xF0
# define KEYBOARD_CMD_LED		0xED

# define KEYBOARD_SCANCODE_GET		0x00
# define KEYBOARD_SCANCODE_SET_1	0x01
# define KEYBOARD_SCANCODE_SET_2	0x02
# define KEYBOARD_SCANCODE_SET_3	0x03

# define KEYBOARD_STATUS_RDOK		(1 << 0)
# define KEYBOARD_STATUS_WRNOTOK	(1 << 1)

char keyboard_next_input(void);

#endif /* !_DRIVER_KEYBOARD_H_ */
