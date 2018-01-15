/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_HEXDUMP_H_
# define _KERNEL_HEXDUMP_H_

# include <chaosdef.h>

# define HEXDUMP_WIDTH		16

void	hexdump(uchar const *start, size_t size);

#endif /* !_KERNEL_HEXDUMP_H_ */
