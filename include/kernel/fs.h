/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_FS_H_
# define _KERNEL_FS_H_

# include <chaosdef.h>
# include <chaoserr.h>
# include <kernel/mutex.h>

/* The type a file can be */
# define FS_REGULAR_FILE	0b0000
# define FS_DIRECTORY		0b0001
# define FS_CHARDEVICE		0b0010
# define FS_BLOCKDEVICE		0b0100

struct bdev;
struct dirent;
struct file_handle;
struct dir_handle;

/*
** Api that a filesystem must (at least partialy) implement
*/
struct fs_api
{
	status_t (*mount)(struct bdev *, void **);
	status_t (*unmount)(void *);
	status_t (*open)(struct file_handle *, char const *);
	status_t (*close)(struct file_handle *);
	status_t (*opendir)(struct dir_handle *);
	status_t (*readdir)(struct dir_handle *, struct dirent *);
	void (*closedir)(struct dir_handle *);
};

/*
** Handle on a mounted filesystem
*/
struct fs_mount
{
	char *path;
	struct bdev *device;
	void *fs_data;
	struct fs_api *api;

	int ref_count;
	struct mutex lock;
};

/*
** Handle on an opened file
** Used for reading, writing etc.
*/
struct file_handle
{
	struct fs_mount *mount;
	uint type;
	void *file_data;
};

/*
** Handle on an opened dir
*/
struct dir_handle
{
	struct file_handle *file_handle;
	void *dir_data;
};

/*
** A directory entry, as returned by the readdir syscall.
*/
struct dirent
{
	char name[256];
	bool dir;
};

/* Generic fs functions */
status_t		fs_mount(char const *p, char const *fs, char const *dev);
status_t		fs_unmount(char const *path);
status_t		fs_open(char const *path, struct file_handle **handle);
status_t		fs_close(struct file_handle *file_handle);
status_t		fs_opendir(struct file_handle *handle, struct dir_handle **dir_handle);
status_t		fs_readdir(struct dir_handle *handle, struct dirent *dirent);
status_t		fs_closedir(struct dir_handle *dir_handle);


struct fs_hook
{
	char const *name;
	struct fs_api *api;
};

# define NEW_FILESYSTEM(n, a)						\
	__aligned(sizeof(void*)) __used __section("fs_hook")		\
	static const struct fs_hook _fs_hook_##n = {			\
		.name = #n,						\
		.api = a,						\
	}

#endif /* !_KERNEL_FS_H_ */
