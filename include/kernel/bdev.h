/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_BDEV_H_
# define _KERNEL_BDEV_H_

# include <kernel/rwlock.h>
# include <chaoserr.h>

struct bdev
{
	char *name;

	uint ref_count; /* ref counter */
	struct rwlock lock; /* Lock to this device */

	/* api */
	ssize_t (*read)(struct bdev *, void *buf, size_t offset, size_t len);
	ssize_t (*write)(struct bdev *, void const *buf, size_t offset, size_t len);
	void (*close)(struct bdev *);
};

status_t		bdev_init(struct bdev *, char const *);
struct bdev		*bdev_open(char const *name);
void			bdev_close(struct bdev *);
status_t		bdev_register(struct bdev *bdev);
void			bdev_unregister(struct bdev *bdev);

/*
** Reads a block device and stores the content in the given buffer.
** The device must be locked as reader.
*/
static inline ssize_t
bdev_read(struct bdev *dev, void *buff, size_t offset, size_t len)
{
	return (dev->read(dev, buff, offset, len));
}

/*
** Writes in a block device using the content in the given buffer.
** The device must be locked as writter.
*/
static inline ssize_t
bdev_write(struct bdev *dev, void const *buff, size_t offset, size_t len)
{
	return (dev->write(dev, buff, offset, len));
}

#endif /* !_KERNEL_BDEV_H_ */
