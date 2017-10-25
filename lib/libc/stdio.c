/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <drivers/vga.h>
#include <stdio.h>
#include <printf.h>
#include <limits.h>

int
puts(char const *str)
{
	return (vga_puts(str));
}

int
putsn(char const *str, size_t n)
{
	return (vga_putsn(str, n));
}

int
putchar(int c)
{
	return (vga_putchar(c));
}

int
printf(char const *fmt, ...)
{
	va_list va;
	int out;

	va_start(va, fmt);
	out = vprintf(fmt, va);
	va_end(va);
	return (out);
}

int
sprintf(char *str, char const *fmt, ...)
{
	va_list va;
	int out;

	va_start(va, fmt);
	out = vsprintf(str, fmt, va);
	va_end(va);
	return (out);
}

int
snprintf(char *str, size_t len, char const *fmt, ...)
{
	va_list va;
	int out;

	va_start(va, fmt);
	out = vsnprintf(str, len, fmt, va);
	va_end(va);
	return (out);
}

static int
vprintf_output(char const *buffer, size_t buffer_len, void *p)
{
	struct printf_output_args *out;

	out = p;
	while (buffer_len > 0)
	{
		out->ret += putchar(*buffer);
		++buffer;
		--buffer_len;
	}
	return (out->ret);
}

int
vprintf(char const *fmt, va_list va)
{
	struct printf_output_args args;

	args.ret = 0;
	return (printf_formatter(fmt, va, &vprintf_output, &args));
}

int
vsprintf(char *str, char const *fmt, va_list va)
{
	return (vsnprintf(str, INT_MAX, fmt, va));
}

static int
vsnprintf_output(char const *buffer, size_t buffer_len, void *p)
{
	struct vsnprintf_output_args *out;
	size_t count;

	out = p;
	count = 0;
	while (count < buffer_len)
	{
		if (out->pos < out->len) {
			out->str[out->pos++] = *buffer;
		}
		buffer++;
		count++;
	}
	return (out->pos);
}

int
vsnprintf(char *str, size_t len, char const *fmt, va_list ap)
{
	struct vsnprintf_output_args out;
	int w;

	out.str = str;
	out.len = len;
	out.pos = 0u;
	w = printf_formatter(fmt, ap, &vsnprintf_output, (void *)&out);
	return (w);
}
