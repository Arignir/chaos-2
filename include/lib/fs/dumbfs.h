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
	uint32 entry_size; /* size of entry, including name, but excluding this header */
	uint32 file_size; /* size of file */
};

struct dumbfs_file
{
	struct dumbfs_file_entry header;
	size_t bdev_offset;
	size_t seek_offset;
};

struct dumbfs_dir
{
	size_t bdev_offset;
	size_t file_index;
};

struct fs_dumb
{
	struct bdev *bdev;
	uint32 nb_files;
};

#endif /* !_LIB_FS_DUMBFS_H_ */
