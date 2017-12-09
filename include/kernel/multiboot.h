/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_MULTIBOOT_H_
# define _KERNEL_MULTIBOOT_H_

# include <kernel/memory.h>
# include <multiboot2.h>

struct multiboot_info
{
	char const *cmd_line;
	char const *bootloader;
	uintptr mem_start;
	uintptr mem_stop;
	multiboot_memory_map_t *mmap;
	multiboot_memory_map_t *mmap_end;
	size_t mmap_entry_size;
};

extern struct cmd_options cmd_options;
extern struct multiboot_info multiboot_infos;

#endif /* !_KERNEL_MULTIBOOT_H_ */
