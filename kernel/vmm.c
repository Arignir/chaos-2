/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

/*
** Architecture independant virtual memory management
*/

#include <kernel/vmm.h>
#include <kernel/kalloc.h>
#include <kernel/initrd.h>
#include <lib/bdev/mem_bdev.h>
#include <arch/linker.h>

/*
** Maps contiguous virtual addresses to a random physical addresses.
** If the given virtual address is NULL, then the kernel chooses
** the destination address.
**
** Size must be page aligned.
**
** If this is a remap, the old frame will be freed. If you care about the old frames,
** seperate this in two calls: one to munmap() with the MUNMAP_DONTFREE flag, and the
** other one to mmap().
**
** Returns the virtual address holding the mapping, or NULL if
** it fails.
** In case of error, the state is like it was before the call.
**
*/
virtaddr_t
mmap(virtaddr_t va, size_t size, mmap_flags_t flags)
{
	uchar *ori_va;
	status_t s;

	assert_vmm(IS_PAGE_ALIGNED(va));
	assert_vmm(IS_PAGE_ALIGNED(size));

	ori_va = va;
	if (unlikely(!va)) {
		panic("mmap with a NULL address not implemented yet.\n");
	}
	else {
		while ((uchar *)va < ori_va + size)
		{
			s = vmm_map_virtual(va, flags);
			if (s != OK) {
				if (s != ERR_ALREADY_MAPPED) {
					munmap(
						ori_va,
						(uchar *)va - ori_va,
						MUNMAP_DEFAULT
					);
				}
				goto err;
			}
			va = (uchar *)va + PAGE_SIZE;
		}
	}

	return (ori_va);
err:
	return (NULL);
}

/*
** Maps contiguous virtual addresses to the given physical addresses.
** If the given virtual address is NULL, then the kernel chooses
** the destination address.
**
** Doesn't mark the given physical range as allocated. This must be done
** before the call to mmap_device.
**
** Size must be page aligned.
**
** If this is a remap, the old frame will be freed. If you care about the old frames,
** seperate this in two calls: one to munmap() with the MUNMAP_DONTFREE flag, and the
** other one to mmap().
**
** Returns the virtual address holding the mapping, or NULL if
** it fails.
** In case of error, the state is like it was before the call.
*/
virtaddr_t
mmap_device(virtaddr_t va, physaddr_t pa, size_t size, mmap_flags_t flags)
{
	uchar *ori_va;
	status_t s;

	assert_vmm(IS_PAGE_ALIGNED(va));
	assert_vmm(IS_PAGE_ALIGNED(pa));
	assert_vmm(IS_PAGE_ALIGNED(size));

	ori_va = va;
	if (unlikely(!va)) {
	panic("mmap_device with a NULL address is not implemented yet.\n");
	}
	else {
		while ((uchar *)va < ori_va + size)
		{
			s = vmm_map_physical(va, pa, flags);
			if (s != OK) {
				if (s != ERR_ALREADY_MAPPED) {
					munmap(
						ori_va,
						(uchar *)va - ori_va,
						MUNMAP_DONTFREE
					);
				}
				goto err;
			}
			va = (uchar *)va + PAGE_SIZE;
			pa += PAGE_SIZE;
		}
	}

	return (ori_va);
err:
	return (NULL);
}

/*
** Unmaps the contiguous virtual addresses given in parameters.
** Does nothing if one of the given pages is actually not mapped.
*/
void
munmap(virtaddr_t va, size_t size, munmap_flags_t flags)
{
	virtaddr_t ori_va;

	assert_vmm(IS_PAGE_ALIGNED(va));
	assert_vmm(IS_PAGE_ALIGNED(size));

	ori_va = va;
	while ((uchar *)va < (uchar *)ori_va + size)
	{
		vmm_unmap(va, flags);
		va = (uchar *)va + PAGE_SIZE;
	}
}

/*
** Returns true if the given buffer is an invalid user-provided buffer
** (Not mapped, do not belongs to user etc.)
*/
bool
vmm_validate_buffer(void const *buffer, size_t len)
{
	uchar const *buff_start;
	uchar const *buff_end;

	buff_start = (uchar const *)ROUND_DOWN((uintptr)buffer, PAGE_SIZE);
	buff_end = (uchar const *)ROUND_DOWN((uintptr)buffer + len, PAGE_SIZE);
	do {
		if (!vmm_is_mapped_in_userspace(buff_start)) {
			return (true);
		}
	} while (buff_start != buff_end);
	return (false);
}

/*
** Calculates the length of the given string, ensuring it belongs to valid userspace
** memory.
** Returns true if the memory is not valid, false otherwise. Length is stored in 'len'.
** Len can be NULL, in which case the length is not stored.
*/
bool
vmm_validate_str(char const *str, size_t *len)
{
	char *str_align;
	char const *str_start;

	str_start = str;
	str_align = (void *)ROUND_DOWN((uintptr)str, PAGE_SIZE);
	while (true)
	{
		if (!vmm_is_mapped_in_userspace(str_align)) {
			return (true);
		}
		while (str < str_align + PAGE_SIZE) {
			if (*str == '\0') {
				goto end;
			}
			++str;
		}
		str_align += PAGE_SIZE;
	}
end:
	if (len) {
		*len = str - str_start;
	}
	return (false);
}

/*
** Initializes the arch-independant part of the virtual memory manager.
** Called by arch-dependant vmm initializer.
*/
void
vmm_init(void)
{
	struct initrd_virt const *virt;

	kalloc_init();
	if (initrd_is_present()) {
		assert_eq(initrd_map(), OK);
		virt = initrd_get_virtual();
		assert_eq(register_membdev("initrd", virt->start, virt->len), OK);
	}
}
