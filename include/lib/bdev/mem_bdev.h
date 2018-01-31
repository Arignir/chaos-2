/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _LIB_BDEV_MEM_BDEV_H_
# define _LIB_BDEV_MEM_BDEV_H_

# include <kernel/bdev.h>

struct mem_bdev
{
	struct bdev bdev;	/* Base device */
	void *ptr;		/* Pointer to this device content */
	size_t len;		/* Length of the content */
};

status_t		register_membdev(char const *, void *, size_t);

#endif /* !_LIB_BDEV_MEM_BDEV_H_ */
