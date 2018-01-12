/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/elf.h>
#include <kernel/exec.h>
#include <kernel/vaspace.h>
#include <kernel/thread.h>
#include <kernel/cpu.h>
#include <string.h>

#include <stdio.h> /* TODO FIXME For debugging */

#if defined(__i386__)

/*
** Tests if [off;off + len[ is contained in [elf_start; elf_start + elf_len[.
*/
static bool
is_in_range(uchar const *elf_start, size_t elf_len, size_t off, size_t len)
{
	return (elf_start + off < elf_start + elf_len
			&& elf_start + off + len < elf_start + elf_len);
}

/*
** Identifies an elf file and ensures that it corresponds to the current system.
*/
static status_t
elf_identify(uchar const *start, size_t len, void **_ __unused)
{
	struct Elf32_Ehdr *header;

	if (!is_in_range(start, len, 0, sizeof(struct Elf32_Ehdr))) {
		return (ERR_INVALID_ARGS);
	}
	header = (struct Elf32_Ehdr *)start;
	if (start[EI_MAG0] != ELFMAG0
		|| start[EI_MAG1] != ELFMAG1
		|| start[EI_MAG2] != ELFMAG2
		|| start[EI_MAG3] != ELFMAG3
		|| start[EI_VERSION] != EV_CURRENT
#if defined(__i386__)
		|| start[EI_CLASS] != ELFCLASS32
		|| start[EI_DATA] != ELFDATA2LSB
		|| header->e_machine != EM_386
		|| header->e_flags != 0
#endif /* defined(__i386__) */
		|| header->e_type != ET_EXEC
		|| header->e_version != EV_CURRENT
		|| header->e_entry == 0
	) {
		return (ERR_INVALID_ARGS);
	}
	return (OK);
}

/*
** Maps a given program segment memory.
**
** Arguments are the program header entry and a pointer to the segment's
** content.
**
** This tests if the given segment overlaps an existing one and maps
** it to the current virtual address space, with the given content.
**
** In case of error (out of memory, segment overlapping etc.), the memory is
** left intact.
**
** This asserts 'ph->p_memsz' is non null.
*/
static status_t
elf_map_program_segment(struct Elf32_Phdr *ph, uchar const *seg_start)
{
	status_t s;
	virtaddr_t v;
	mmap_flags_t flags;

	flags = MMAP_USER;
	flags |= MMAP_EXEC * !!(ph->p_flags & PF_X);
	flags |= MMAP_WRITE * !!(ph->p_flags & PF_W);
	v = mmap(
		(uchar *)ROUND_DOWN(ph->p_vaddr, PAGE_SIZE),
		PAGE_ALIGN(ph->p_memsz),
		flags
	);
	if (!v) {
		return (ERR_NO_MEMORY);
	}
	s = vaspace_add_vseg(
		current_vaspace(),
		v,
		(uchar *)v + PAGE_ALIGN(ph->p_memsz) - PAGE_SIZE
	);
	if (s != OK) {
		munmap(v, PAGE_ALIGN(ph->p_memsz), MUNMAP_DEFAULT);
		return (s);
	}
	memcpy((uchar *)ph->p_vaddr, seg_start, ph->p_filesz);
	memset((uchar *)ph->p_vaddr + ph->p_filesz, 0, ph->p_memsz - ph->p_filesz);
	return (OK);
}

/*
** Maps the elf file in memory.
** In case of error, the memory is left intact.
**
** Arguments are the elf file's content and it's length.
**
** This function may fail for various reasons: out of memory or an invalid/unsupported
** elf file.
*/
static status_t
elf_map(uchar const *start, size_t len, void **_ __unused)
{
	struct Elf32_Ehdr *header;
	struct Elf32_Phdr *ph;
	size_t i;
	Elf32_Addr base;
	Elf32_Addr base_end;
	status_t s;

	base = 0;
	base_end = 0;
	header = (struct Elf32_Ehdr *)start;
	if (!is_in_range(start, len, header->e_phoff,
				header->e_phentsize * header->e_phnum)) {
		return (ERR_INVALID_ARGS);
	}
	for (i = 0; i < header->e_phnum; ++i) {
		ph = (struct Elf32_Phdr *)(start + header->e_phoff + i * header->e_phentsize);
		if (ph->p_type == PT_LOAD
			&& is_in_range(start, len, ph->p_offset, ph->p_filesz)
			&& ph->p_memsz
			&& ph->p_filesz <= ph->p_memsz
			&& ph->p_vaddr > base
		) {
			s = elf_map_program_segment(
				ph,
				start + ph->p_offset
			);
			if (s) {
				munmap(
					(uchar *)ROUND_DOWN(base, PAGE_SIZE),
					ROUND_DOWN(base_end, PAGE_SIZE) - ROUND_DOWN(base, PAGE_SIZE) + PAGE_SIZE,
					MUNMAP_DEFAULT
				);
				return (s);
			}
			if (!base) {
				base = ph->p_vaddr;
			}
			base_end = ph->p_vaddr + ph->p_memsz;
		}
	}
	return (OK);
}

/*
** Executes the given ELF by building up the thread stack and jumping to the
** main address.
*/
static status_t
elf_exec(uchar const *start, size_t _ __unused , void **__ __unused)
{
	struct Elf32_Ehdr *header;
	struct thread *t;
	virtaddr_t stack_top;
	status_t s;

	t = current_cpu()->thread;
	header = (struct Elf32_Ehdr *)start;
	if ((s = thread_create_stacks())) {
		return (s);
	}

#if KCONFIG_DEBUG_THREAD || KCONFIG_DEBUG_VMM
	printf("Executing %p.\nThread Stack: %p-%p\nKernel stack: %p-%p\n",
		(void *)header->e_entry,
		t->stack,
		t->stack_top,
		t->kstack,
		t->kstack_top
	);
	printf("Address Space:\n");
	vaspace_dump(current_vaspace());
#endif /* KCONFIG_DEBUG_THREAD */

	arch_set_kernel_stack((uintptr)t->kstack_top);
	stack_top = t->stack_top;

	/* Release lock of the current thread (We're about to execute it) */
	rwlock_release_write(&t->vaspace->rwlock);
	rwlock_release_write(&t->rwlock);

	arch_jump_to_userspace(stack_top, (void (*)(void))header->e_entry);
	panic("Unreachable elf_exec()");
}

NEW_EXEC_FORMAT(elf, &elf_identify, &elf_map, &elf_exec, NULL);

#endif /* defined(__i386__) */
