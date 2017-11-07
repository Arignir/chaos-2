/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_CHECKSUM_H_
# define _KERNEL_CHECKSUM_H_

# include <chaosdef.h>

/* Functions to facilitate kernel & drivers code */

uint8	checksum8(void *s, size_t l);
uint16	checksum16(void *s, size_t l);
uint32	checksum32(void *s, size_t l);

#endif /* !_KERNEL_CHECKSUM_H_ */
