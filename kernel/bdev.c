/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/bdev.h>
#include <kernel/kalloc.h>
#include <string.h>

static struct bdev **bdev_tab = NULL;
static size_t bdev_tab_size = 0;
struct rwlock bdev_tab_lock = RWLOCK_DEFAULT;

static inline void
bdev_inc_ref(struct bdev *bdev)
{
	++bdev->ref_count;
}

static inline void
bdev_dec_ref(struct bdev *bdev)
{
	struct bdev **tmp;

	--bdev->ref_count;
	if (bdev->ref_count == 0) {
		rwlock_acquire_write(&bdev_tab_lock);
		if (bdev->close) {
			bdev->close(bdev);
		}
		kfree(bdev->name);
		--bdev_tab_size;
		tmp = krealloc(bdev_tab, sizeof(*bdev_tab) * (--bdev_tab_size));
		if (tmp) {
			bdev_tab = tmp;
		}
		rwlock_release_write(&bdev_tab_lock);
	}
}

/*
** Initilises a block device.
*/
status_t
bdev_init(struct bdev *bdev, char const *name)
{
	bdev->name = strdup(name);
	if (bdev->name == NULL) {
		return (ERR_NO_MEMORY);
	}
	bdev->ref_count = 0;
	rwlock_init(&bdev->lock);
	return (OK);
}

/*
** Opens a block device
** The returned device is already locked as writter.
*/
struct bdev *
bdev_open(char const *name)
{
	size_t i;
	struct bdev *bdev;

	rwlock_acquire_read(&bdev_tab_lock);
	for (i = 0; i < bdev_tab_size; ++i) {
		bdev = bdev_tab[i];
		rwlock_acquire_read(&bdev->lock);
		if (!strcmp(name, bdev->name)) {
			rwlock_release_read(&bdev->lock);
			rwlock_acquire_write(&bdev->lock);
			if (!strcmp(name, bdev->name)) {
				bdev_inc_ref(bdev);
				goto ret;
			}
			rwlock_release_write(&bdev->lock);
		} else {
			rwlock_release_read(&bdev->lock);
		}
	}
	bdev = NULL;
ret:
	rwlock_release_read(&bdev_tab_lock);
	return (bdev);
}

/*
** Closes the given device.
** The device must be locked as writter.
*/
void
bdev_close(struct bdev *bdev)
{
	bdev_dec_ref(bdev);
}

/*
** Registers the given block device.
** The device must be locked as writter.
*/
status_t
bdev_register(struct bdev *bdev)
{
	struct bdev **tmp;

	rwlock_acquire_write(&bdev_tab_lock);
	tmp = krealloc(bdev_tab, sizeof(*bdev_tab) * (bdev_tab_size + 1));
	if (!tmp) {
		rwlock_release_write(&bdev_tab_lock);
		return (ERR_NO_MEMORY);
	}
	bdev_tab = tmp;
	bdev_tab[bdev_tab_size] = bdev;
	++bdev_tab_size;
	bdev_inc_ref(bdev);
	rwlock_release_write(&bdev_tab_lock);
	return (OK);
}

/*
** Unregisters the given block device.
** The device must be locked as writter.
*/
void
bdev_unregister(struct bdev *bdev)
{
	bdev_dec_ref(bdev);
}
