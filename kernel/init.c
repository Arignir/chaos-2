/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/init.h>

extern struct init_hook const __start_chaos_init[] __weak;
extern struct init_hook const __stop_chaos_init[] __weak;

/*
** Finds the next uncalled hook which init level is between the two given.
*/
static struct init_hook const *
find_next_hook(struct init_hook const *last,
		enum init_level level,
		enum init_level end_level)
{
	struct init_hook const *hook;
	struct init_hook const *found;
	bool seen_last;

	found = NULL;
	seen_last = false;
	for (hook = __start_chaos_init; hook < __stop_chaos_init; ++hook)
	{
		seen_last |= (hook == last);
		if (hook->level < level
			|| hook->level > end_level
			|| (found && found->level <= hook->level)) {
			continue;
		}
		if (hook->level > level) {
			found = hook;
			continue;
		}
		if (hook->level == level && seen_last && hook != last) {
			found = hook;
			break;
		}
	}
	return (found);
}

/*
** Inits all levels between the two one given in parameter, included
*/
void
trigger_init_levels(enum init_level start, enum init_level end)
{
	struct init_hook const *hook;

	hook = find_next_hook(NULL, start, end);
	while (hook != NULL) {
		hook->hook();
		hook = find_next_hook(hook, hook->level, end);
	}
}
