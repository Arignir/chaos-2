/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/kalloc.h>
#include <kernel/bdev.h>
#include <lib/fs/dumbfs.h>

static status_t
dumbfs_mount(struct bdev *bdev, void **fscookie)
{
	struct fs_dumb *dumb;
	status_t err;

	if (!(dumb = kalloc(sizeof(*dumb)))) {
		return (ERR_NO_MEMORY);
	}

	if (bdev_read(bdev, &dumb->nb_files, 0, sizeof(uint32)) != sizeof(uint32)) {
		err = ERR_BAD_DEVICE;
		goto err;
	}
	dumb->bdev = bdev;
	*fscookie = dumb;
	return (OK);
err:
	kfree(dumb);
	return (err);
}

static status_t
dumbfs_unmount(void *cookie)
{
	kfree(cookie);
	return (OK);
}

static status_t
dumbfs_open(
	void *fscookie __unused /* TODO */,
	char const *path __unused /* TODO */,
	struct filehandler *handler __unused /* TODO */
)
{
	/* TODO */
	return (ERR_NOT_FOUND);
}

static status_t
dumbfs_close(
	void *fscookie __unused /* TODO */,
	struct filehandler *handler __unused /* TODO */
)
{
	return (OK);
}

static status_t
dumbfs_readdir(
	void *fscookie __unused /* TODO */,
	void *data __unused /* TODO */,
	struct dirent *dirent __unused /* TODO */
)
{
	return (ERR_NOT_FOUND);
}

static struct fs_api dumbfs_api =
{
	.mount = &dumbfs_mount,
	.unmount = &dumbfs_unmount,
	.open = &dumbfs_open,
	.close = &dumbfs_close,
	.readdir = &dumbfs_readdir,
};

NEW_FILESYSTEM(dumbfs, &dumbfs_api);
