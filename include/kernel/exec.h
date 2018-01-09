/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_EXEC_H_
# define _KERNEL_EXEC_H_

# include <chaosdef.h>
# include <chaoserr.h>

typedef status_t (*identify_funcptr)(uchar const *, size_t, void **);
typedef status_t (*map_funcptr)(uchar const *, size_t, void **);
typedef status_t (*exec_funcptr)(uchar const *, size_t, void **);
typedef void (*cleanup_funcptr)(void *);

struct exec_format
{
	char const *name;
	identify_funcptr identify_cb;
	map_funcptr map_cb;
	exec_funcptr exec_cb;
	cleanup_funcptr cleanup_cb;
};

# define NEW_EXEC_FORMAT(n, i, m, e, c)					\
	__aligned(sizeof(void *)) __used __section("exec_format")	\
	static struct exec_format const _exec_format_struct_##n = {	\
		.name = #n,						\
		.identify_cb = (i),					\
		.map_cb = (m),						\
		.exec_cb = (e),						\
		.cleanup_cb = (c),					\
	}

status_t	exec(uchar const *file_content, size_t file_size);

#endif /* !_KERNEL_EXEC_H_ */
