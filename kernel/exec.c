/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/exec.h>

extern struct exec_format const __start_exec_format[] __weak;
extern struct exec_format const __stop_exec_format[] __weak;

/*
** Tries to execute the given file content.
**
** Goes through all available executable formats and tries them one by one.
*/
status_t
exec(uchar const *file_content, size_t file_size)
{
	void *tmp;
	status_t s;
	struct exec_format const *format;

	format = __start_exec_format;
	while (format < __stop_exec_format) {
		if (format->identify_cb(file_content, file_size, &tmp) == OK) {
			if ((s = format->map_cb(file_content, file_size, &tmp)) == OK) {
				s = format->exec_cb(file_content, file_size, &tmp);
			}
			if (format->cleanup_cb) {
				format->cleanup_cb(tmp);
			}
			return (s);
		}
		++format;
	}
	return (ERR_INVALID_ARGS);
}
