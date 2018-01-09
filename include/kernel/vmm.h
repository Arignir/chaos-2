/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_VMM_H_
# define _KERNEL_VMM_H_

# include <kconfig.h>
# include <kernel/memory.h>
# include <chaoserr.h>

/* Debug assertions for vmm */
# if KCONFIG_DEBUG_VMM
#  define assert_vmm(x)		assert(x)
# else
#  define assert_vmm(x)
# endif /* KCONFIG_DEBUG_VMM */

/* Arch-indepentant flags for mmap */
/* Warning: depending on the architecture, MMAP_WRITE and MMAP_EXEC may be exclusive. */
# define MMAP_DEFAULT		0b00000000	/* Kernel space, read only */
# define MMAP_USER		0b00000001	/* Page belongs to user space */
# define MMAP_READONLY		0b00000000	/* Page is read-only */
# define MMAP_WRITE		0b00000010	/* Page is writtable */
# define MMAP_EXEC		0b00000100	/* Page is executable */
# define MMAP_REMAP		0b00001000	/* Remap if va is already taken */

/* The integer type corresponding to the flags above */
typedef uint			mmap_flags_t;

/* Arch-indepentant flags for munmap */
# define MUNMAP_DEFAULT		0b00000000	/* Free frames */
# define MUNMAP_DONTFREE	0b00000001	/* Do not free the corresponding frames */

/* The integer type corresponding to the flags above */
typedef uint			munmap_flags_t;

/*
** All the following functions must be implemented in the
** arch-dependent part of the kernel.
**
** These functions should NOT be called directly outside the virtual memory manager.
** They are primitives that does not lock context, so bad things will happen in
** a multi-threaded context. Use more high-level functions instead, like mmap().
*/

/* Used for debugging purposes. Dumps the memory state */
void		vmm_dump_mem(void);
void		vmm_dump_user_mem(void);

/* Tells if the given virtual address is mapped. */
bool		vmm_is_mapped(virtaddr_t);

/* Returns the frame that the given address is mapped to. */
physaddr_t	vmm_get_frame(virtaddr_t va);

/* Maps a physical address to a virtual one. */
status_t	vmm_map_physical(virtaddr_t, physaddr_t, mmap_flags_t);

/* Maps the given virtual address to a random physical addresses. */
status_t	vmm_map_virtual(virtaddr_t va, mmap_flags_t);

/* Unmaps a virtual address */
void		vmm_unmap(virtaddr_t va, munmap_flags_t);

/*
** All the following functions are arch-independant and can be called anywhere.
*/

void		vmm_init(void);

virtaddr_t	mmap(virtaddr_t va, size_t size, mmap_flags_t);
virtaddr_t	mmap_device(virtaddr_t va, physaddr_t pa, size_t size, mmap_flags_t);
void		munmap(virtaddr_t va, size_t size, munmap_flags_t);

#endif /* !_KERNEL_VMM_H_ */
