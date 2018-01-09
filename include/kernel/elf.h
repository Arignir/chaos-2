/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_ELF_H_
# define _KERNEL_ELF_H_

# include <chaosdef.h>
# include <chaoserr.h>
# include <stdint.h>

/* Standard ELF types */
typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef uint32_t Elf32_Sword;
typedef uint32_t Elf32_Word;

/* Elf Identification Indexes */
# define EI_MAG0	0
# define EI_MAG1	1
# define EI_MAG2	2
# define EI_MAG3	3
# define EI_CLASS	4
# define EI_DATA	5
# define EI_VERSION	6
# define EI_PAD		7
# define EI_NIDENT	16

/* Elf Magic value bytes */
# define ELFMAG0	'\x7f'
# define ELFMAG1	'E'
# define ELFMAG2	'L'
# define ELFMAG3	'F'

/* Elf Class byte */
# define ELFCLASSNONE	0
# define ELFCLASS32	1
# define ELFCLASS64	2

/* Elf Data enconding byte */
# define ELFDATANONE	0
# define ELFDATA2LSB	1
# define ELFDATA2MSB	2

/* Elf versions */
# define EV_NONE	0
# define EV_CURRENT	1

/* Elf machine */
# define EM_NONE	0
# define EM_M32		1
# define EM_SPARC	2
# define EM_386		3
# define EM_68K		4
# define EM_88K		5
# define EM_860		7
# define EM_MIPS	8

/* Elf type */
# define ET_NONE	0
# define ET_REL		1
# define ET_EXEC	2
# define ET_DYN		3
# define ET_CORE	4
# define ET_LOPROC	0xff00
# define ET_HIPROC	0xffff

/* Elf segment type */
# define PT_NULL	0
# define PT_LOAD	1
# define PT_DYNAMIC	2
# define PT_INTERP	3
# define PT_NOTE	4
# define PT_SHLIB	5
# define PT_PHDR	6
# define PT_LOPROC	0x70000000
# define PT_HIPROC	0x7fffffff

/* Elf Program Header flags */
# define PF_X		(1 << 0)
# define PF_W		(1 << 1)
# define PF_R		(1 << 2)

struct Elf32_Ehdr
{
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
};

struct Elf32_Phdr
{
	Elf32_Word p_type;
	Elf32_Off p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	Elf32_Word p_filesz;
	Elf32_Word p_memsz;
	Elf32_Word p_flags;
	Elf32_Word p_align;
};

static_assert(sizeof(struct Elf32_Ehdr) == 52);
static_assert(sizeof(struct Elf32_Phdr) == 32);

#endif /* !_KERNEL_ELF_H_ */
