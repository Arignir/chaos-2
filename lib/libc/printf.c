/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <printf.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static char const digits_low[16] = "0123456789abcdef";
static char const digits_upp[16] = "0123456789ABCDEF";

/*
** Returns a pointer to the beginning of the number written in `buff`.
** The characters of `buff` before the returned pointer are not accessed
** by this function. `buff[0]` may be undefined.
*/
static char *
long_to_string(char *buff, size_t pos, ulong nb, uint base, uint flags, char *sign)
{
	bool neg;
	int digit;
	char const *digits;

	neg = false;
	digits = (flags & PRINTF_CAPS) ? digits_upp : digits_low;
	if ((flags & PRINTF_SIGNED_INT) && (long)nb < 0) {
		neg = true;
		nb *= -1; /* Fuck you LONG_MIN ! */
	}
	buff[--pos] = '\0';
	while (nb >= base) {
		digit = nb % base;
		nb /= base;
		buff[--pos] = digits[digit];
	}
	buff[--pos] = digits[nb];

	if (flags & PRINTF_SHOW_BASE)
	{
		if (base == 16) {
			buff[--pos] = (flags & PRINTF_CAPS) ? 'X' : 'x';
		} else if (base == 2) {
			buff[--pos] = (flags & PRINTF_CAPS) ? 'B' : 'b';
		}
		buff[--pos] = '0';
	}
	if (neg) {
		*sign = '-';
	} else if (flags & PRINTF_SHOW_SIGN) {
		*sign = '+';
	} else if (flags & PRINTF_BLANK) {
		*sign = ' ';
	} else {
		*sign = '\0';
	}
	return (buff + pos);
}

static char const* const units[] =
{
  "B",
  "KB",
  "MB",
  "GB",
  "TB",
  "PB",
  "EB",
  "ZB",
  "YB",
  NULL
};

/*
** As weird as long_to_string().
*/
static char *
long_to_readable(char *buff, size_t buffsize, ulong nb, uint base, uint flags, char *sign)
{
	uint i;
	size_t c;
	size_t len;
	char *out;

	i = 0;
	while (nb > 1024ul && units[i]) {
		nb /= 1024u;
		++i;
	}
	len = strlen(units[i]);
	out = long_to_string(buff, buffsize - strlen(units[i]), nb, base, flags, sign);
	c = 0;
	while (c < len)
	{
		buff[buffsize - 1 - len + c] = units[i][c];
		++c;
	}
	buff[buffsize - 1] = '\0';
	return (out);
}

/*
** This printf engine is far from perfect, it has been done quickly
** so that we can move forward easily.
** It does not support floating point, but it adds some custom options:
**	- %y (boolean)
**	- %b (base 2)
**	- %r (human-readable storage-size)
*/
int
printf_formatter(char const *fmt, va_list va, printf_output_func out, void *params)
{
	size_t string_len;
	char const *s;
	char nbstr[256];
	char c_str[2];
	char c;
	uint flags;
	ulong nb;
	uint base;
	uint formatw;
	char sign;

	for (;;) {
		s = fmt;
		string_len = 0;
		flags = 0u;
		nb = 0;
		base = 10;
		formatw = 0;
		sign = '\0';
		while ((c = *fmt++) != '\0') {
			if (c == '%') {
				break;
			}
			string_len++;
		}
		out(s, string_len, params);
		if (c == '\0') {
			break;
		}

format:
		c = *fmt++;
		switch (c) {
		case '0'...'9':
			if (c == '0' && formatw == 0) {
				flags |= PRINTF_LEAD_ZEROS;
			}
			formatw *= 10;
			formatw += c - '0';
			goto format;
		case '%':
			out("%", 1, params);
			break;
		case '+':
			flags |= PRINTF_SHOW_SIGN;
			goto format;
		case '#':
			flags |= PRINTF_SHOW_BASE;
			goto format;
		case '-':
			flags |= PRINTF_LEFT_FORMAT;
			goto format;
		case ' ':
			flags |= PRINTF_BLANK;
			goto format;
		case 's':
			s = va_arg(va, char const *);
			if (s == NULL) {
				s = "<null>";
			}
			flags &= ~PRINTF_LEAD_ZEROS;
			goto print_output;
		case 'c':
			c_str[0] = va_arg(va, unsigned int);
			c_str[1] = '\0';
			s = c_str;
			flags &= ~PRINTF_LEAD_ZEROS;
			goto print_output;
		case 'l':
			flags |= PRINTF_LONG;
			goto format;
		case 'h':
			if (flags & PRINTF_HALF) {
				flags |= PRINTF_HALFHALF;
			}
			flags |= PRINTF_HALF;
			goto format;
		case 'z':
			flags |= PRINTF_SIZE_T;
			goto format;
		case 't':
			flags |= PRINTF_PTRDIFF_T;
			goto format;
		case 'p':
			flags |= PRINTF_SIZE_T;
			flags |= PRINTF_SHOW_BASE;
			base = 16;
			goto print_unsigned;
		case 'd':
		case 'i':
			nb = (flags & PRINTF_HALFHALF) ? (signed char)va_arg(va, int) :
				(flags & PRINTF_HALF) ? (short)va_arg(va, int) :
				(flags & PRINTF_LONG) ? va_arg(va, long) :
				(flags & PRINTF_PTRDIFF_T) ? va_arg(va, ptrdiff_t) :
				va_arg(va, int);
			flags |= PRINTF_SIGNED_INT;
			s = long_to_string(nbstr, sizeof(nbstr), nb, base, flags, &sign);
			goto print_output;
		case 'r':
			nb = (flags & PRINTF_HALFHALF) ? (signed char)va_arg(va, int) :
				(flags & PRINTF_HALF) ? (short)va_arg(va, int) :
				(flags & PRINTF_LONG) ? va_arg(va, long) :
				(flags & PRINTF_PTRDIFF_T) ? va_arg(va, ptrdiff_t) :
				va_arg(va, int);
			flags |= PRINTF_SIGNED_INT;
			s = long_to_readable(nbstr, sizeof(nbstr), nb, base, flags, &sign);
			goto print_output;
		case 'o':
			base = 8;
			goto print_unsigned;
		case 'b':
			base = 2;
			goto print_unsigned;
		case 'y':
			s = va_arg(va, int) ? "true" : "false";
			goto print_output;
		case 'X':
			flags |= PRINTF_CAPS;
			/* falls through */
		case 'x':
			base = 16;
			/* falls through */
print_unsigned:
		case 'u':
			nb = (flags & PRINTF_HALFHALF) ? (uchar)va_arg(va, int) :
				(flags & PRINTF_HALF) ? (ushort)va_arg(va, int) :
				(flags & PRINTF_LONG) ? va_arg(va, ulong) :
				(flags & PRINTF_SIZE_T) ? va_arg(va, size_t) :
				(flags & PRINTF_PTRDIFF_T) ? (uintptr)va_arg(va, ptrdiff_t) :
				va_arg(va, uint);
			s = long_to_string(nbstr, sizeof(nbstr), nb, base, flags, &sign);
			goto print_output;
		default:
			out("%", 1, params);
			out(&c, 1, params);
		}

		/* No format specified, we should continue */
		continue;

		/* Formats join here to print their output */
print_output:
		string_len = strlen(s);
		if (flags & PRINTF_LEFT_FORMAT) {
			formatw -= (sign != '\0' && formatw > 0);
			if (sign != '\0') {
				out(&sign, 1, params);
			}
			out(s, string_len, params);
			while (formatw > string_len)
			{
				out(" ", 1, params);
				--formatw;
			}
		} else {
			formatw -= (sign != '\0' && formatw > 0);
			if ((flags & PRINTF_LEAD_ZEROS) && sign != '\0') {
				out(&sign, 1, params);
			}
			while (formatw > string_len) {
				out(flags & PRINTF_LEAD_ZEROS ? "0" : " ", 1, params);
				--formatw;
			}
			if (!(flags & PRINTF_LEAD_ZEROS) && sign != '\0') {
				out(&sign, 1, params);
			}
			out(s, string_len, params);
		}
	}
	return (out(NULL, 0, params));
}
