/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _LIB_FS_DUMBFS_H_
# define _LIB_FS_DUMBFS_H_

# include <kernel/fs.h>

struct dumbfs_file_entry
{
	uint32 file_size;
	uchar name[16];
} __aligned;

struct dubfs_file
{
	struct dumbfs_file_entry;
	size_t bdev_offset;
};

struct fs_dumb
{
	struct bdev *bdev;
	uint nb_files;
};

#endif /* !_LIB_FS_DUMBFS_H_ */
