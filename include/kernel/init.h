/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_INIT_H_
# define _KERNEL_INIT_H_

# include <chaosdef.h>
# include <limits.h>

/*
** The different init levels that an init hook can suscribe to.
*/
enum init_level
{
	INIT_LEVEL_EARLIEST		= 0x1,

	INIT_LEVEL_ARCH_EARLY,
	INIT_LEVEL_DRIVER_EARLY,

	INIT_LEVEL_PMM,
	INIT_LEVEL_VMM,

	INIT_LEVEL_ARCH,
	INIT_LEVEL_DRIVERS,

	INIT_LEVEL_LATEST,
};

typedef void(*init_hook_funcptr)(void);

struct init_hook
{
	enum init_level level;
	init_hook_funcptr hook;
	char const *name;
};

void	trigger_init_levels(enum init_level start, enum init_level end);

# define NEW_INIT_HOOK(n, h, l)						\
	__aligned(sizeof(void *)) __used __section("chaos_init")	\
	static struct init_hook const _init_hook_struct_##n = {		\
		.level = l,						\
		.hook = h,						\
		.name = #n,						\
	}

#endif /* !_KERNEL_INIT_H_ */
