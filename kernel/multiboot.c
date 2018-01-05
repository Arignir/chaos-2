/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kconfig.h>
#include <multiboot2.h>
#include <kernel/init.h>
#include <kernel/initrd.h>
#include <kernel/multiboot.h>
#include <string.h>

#if KCONFIG_DEBUG_MULTIBOOT
# include <stdio.h>
#endif /* KCONFIG_DEBUG_MULTIBOOT */

/* The first tag given by multiboot, if any */
struct multiboot_tag *mb_tag = NULL;

/* Informations given to us using multiboot that any driver can use */
struct multiboot_info multiboot_infos;

/*
** Goes through the multiboot structure, parsing it's content
*/
static void
multiboot_load(void)
{
	struct multiboot_tag *tag;

	memset(&multiboot_infos, 0, sizeof(multiboot_infos));
	tag = mb_tag;
	while (tag->type != MULTIBOOT_TAG_TYPE_END)
	{
		switch (tag->type)
		{
		case MULTIBOOT_TAG_TYPE_CMDLINE:
			multiboot_infos.cmd_line = ((struct multiboot_tag_string *)tag)->string;
			break;
		case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
			multiboot_infos.bootloader = ((struct multiboot_tag_string *)tag)->string;
			break;
		case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
			multiboot_infos.mem_start = ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower;
			multiboot_infos.mem_stop = ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper;
			break;
		case MULTIBOOT_TAG_TYPE_MMAP:
			multiboot_infos.mmap = ((struct multiboot_tag_mmap *)tag)->entries;
			multiboot_infos.mmap_entry_size = ((struct multiboot_tag_mmap *)tag)->entry_size;
			multiboot_infos.mmap_end = (multiboot_memory_map_t *)((uchar *)tag + tag->size);
			break;
		case MULTIBOOT_TAG_TYPE_MODULE:
			initrd_set_physical(
				((struct multiboot_tag_module *)tag)->mod_start,
				((struct multiboot_tag_module *)tag)->mod_end
			);
			break;
		}
		tag = (struct multiboot_tag *)((uchar *)tag + ((tag->size + 7) & ~7));
	}
}

static void
multiboot_init(void)
{
#if KCONFIG_DEBUG_MULTIBOOT
	multiboot_memory_map_t *mmap;
#endif /* KCONFIG_DEBUG_MULTBOOT */

	if (mb_tag) {
		multiboot_load();
#if KCONFIG_DEBUG_MULTIBOOT
		printf("Booted from \"%s\"\n", multiboot_infos.bootloader);
		printf("Command line: \"%s\"\n", multiboot_infos.cmd_line);
		printf("Memory: From %p to %p (Size: %r)\n",
				(void *)(multiboot_infos.mem_start * 1024u),
				(void *)(multiboot_infos.mem_stop * 1024u),
				(multiboot_infos.mem_stop - multiboot_infos.mem_start) * 1024u
		);

		printf("Memory Areas:\n");
		mmap = multiboot_infos.mmap;
		while (mmap < multiboot_infos.mmap_end)
		{
			printf("\tFrom %#llx to %#llx (Flag: %u)\n",
					ROUND_DOWN(mmap->addr, PAGE_SIZE),
					ALIGN(mmap->addr + mmap->len, PAGE_SIZE),
					mmap->type
			);
			mmap = (multiboot_memory_map_t *)((uchar *)mmap + multiboot_infos.mmap_entry_size);
		}


#endif /* KCONFIG_DEBUG_MULTIBOOT */
	}
}

NEW_INIT_HOOK(multiboot, &multiboot_init, INIT_LEVEL_MULTIBOOT);
