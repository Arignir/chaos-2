#ifndef DRIVER_KEYBOARD_H_
# define DRIVER_KEYBOARD_H_

# define INT_KEYBOARD			0x21
# define KEYBOARD_IO_PORT		0x60

# define SC1_CAPSLOCK_PRESSED		0x3A
# define SC1_LSHIFT_PRESSED		0x2A
# define SC1_RSHIFT_PRESSED		0x36
# define SC1_LSHIFT_RELEASED            0xAA
# define SC1_RSHIFT_RELEASED            0xB6

char keyboard_next_input(void);

#endif /* !DRIVER_KEYBOARD_H_ */
