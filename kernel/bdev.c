/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/vector.h>
#include <kernel/bdev.h>
#include <string.h>

struct bdev_vector
{
	NEW_VECTOR(struct bdev *);
};

static struct bdev_vector bdev_vector;
static struct rwlock bdev_tab_lock = RWLOCK_DEFAULT;

static inline void
bdev_inc_ref(struct bdev *bdev)
{
	++bdev->ref_count;
}

static inline void
bdev_dec_ref(struct bdev *bdev)
{
	--bdev->ref_count;
	if (bdev->ref_count == 0) {
		rwlock_acquire_write(&bdev_tab_lock);
		if (bdev->close) {
			bdev->close(bdev);
		}
		vector_foreach_count(&bdev_vector, tmp, i) {
			if (*tmp == bdev) { /* Remove it */
				vector_remove(&bdev_vector, i);
				break;
			}
		}
		kfree(bdev->name);
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
	struct bdev *bdev;

	rwlock_acquire_read(&bdev_tab_lock);
	vector_foreach(&bdev_vector, bdev_ptr) {
		bdev = *bdev_ptr;
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
	rwlock_acquire_write(&bdev_tab_lock);
	vector_pushback(&bdev_vector, bdev);
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
