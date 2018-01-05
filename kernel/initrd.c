/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/initrd.h>
#include <kernel/kalloc.h>
#include <kernel/hexdump.h>
#include <stdio.h>

static struct initrd_phys initrd_phys =
{
	.start = NULL_FRAME,
	.end = NULL_FRAME,
};

static struct initrd_virt initrd_virt =
{
	.start = NULL,
	.len = 0,
};


/*
** Maps the initial ramdisk to memory.
** It can then be accessed with a call to initrd_get_virtual().
**
** This function is not thread-safe.
*/
status_t
initrd_map(void)
{
	physaddr_t round_start;
	physaddr_t round_end;

	assert(initrd_is_present());

	printf("Mapping: Start %p End %p\n", initrd_phys.start, initrd_phys.end);
	round_start = PAGE_ALIGN(initrd_phys.start);
	round_end = PAGE_ALIGN(initrd_phys.end);
	printf("RoundMapping: Start %p End %p\n", round_start, round_end);
	initrd_virt.len = initrd_phys.end - initrd_phys.start + 1;
	initrd_virt.start = kalloc_device(
		round_end - round_start + PAGE_SIZE,
		initrd_phys.start
	);
	return (initrd_virt.start ? OK : ERR_NO_MEMORY);
}

/*
** Returns the virtual address and length of the initial ramdisk,
** or NULL and 0 if it wasn't mapped yet.
*/
struct initrd_virt *
initrd_get_virtual(void)
{
	return (&initrd_virt);
}

/*
** Returns the physical start and end addresses of the initial ramdisk, or respectively
** NULL_FRAME if they weren't set before.
**
** These addresses may not be page-aligned.
*/
struct initrd_phys *
initrd_get_physical(void)
{
	return (&initrd_phys);
}

/*
** Sets where in the physical memory the initial ramdisk starts and ends.
** This will NOT lock this memory area, but store them for a future call to
** initrd_map().
*/
void
initrd_set_physical(physaddr_t start, physaddr_t end)
{
	initrd_phys.start = start;
	initrd_phys.end = end;
}

/*
** Returns true if a call to initrd_set_physical() has been made with valid
** arguments, false otherwise.
*/
bool
initrd_is_present(void)
{
	return (initrd_phys.start != NULL_FRAME);
}

/*
** Dumps the initial ramdisk.
** Used for debugging purposes.
*/
__unused
void
initrd_dump(void)
{
	assert(initrd_virt.start);

	printf("Dumping initrd:\n");
	hexdump(initrd_virt.start, initrd_virt.len);
}
