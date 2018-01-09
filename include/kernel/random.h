/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_RANDOM_H_
# define _KERNEL_RANDOM_H_

# include <chaosdef.h>
# include <limits.h>

void	seed_by_time(void);
uchar	rand_uchar(void);
ushort	rand_ushort(void);
uint	rand_uint(void);
ulong	rand_ulong(void);

#endif /* !_KERNEL_RANDOM_H_ */
