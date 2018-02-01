#ifndef DRIVER_KEYBOARD_H_
# define DRIVER_KEYBOARD_H_

# include <chaosdef.h>

# define INT_KEYBOARD			0x21
# define KEYBOARD_IO_PORT		0x60

char
keyboard_next_input(void);

#endif /* !DRIVER_KEYBOARD_H_ */
