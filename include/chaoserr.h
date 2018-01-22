/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _CHAOSERR_H_
# define _CHAOSERR_H_

# include <limits.h>

/*
** This enum contains all the possible error-codes a function can return.
** Error values are always negative.
**
** Feel free to complete it according your needs.
*/
enum				status
{
	OK			= 0,

	__ERR_UNKNOWN		= CHAR_MIN, /* Used to calibrate the next enum values. Should never be used. */

	/* Internal errors */
	ERR_INTERNAL,			/* Internal / unspecified error. Keep this one for extrem cases. */
	ERR_NOT_IMPLEMENTED,		/* Operation is not implemented */
	ERR_NOT_SUPPORTED,		/* Operation is not supported */
	ERR_NO_MEMORY,			/* Operation couldn't complete because it went out of memory */
	ERR_INTERRUPTED,		/* Operation got interrupted */

	/* Parameters errors */
	ERR_INVALID_ARGS,		/* Invaid arguments */
	ERR_OUT_OF_RANGE,		/* One of the argument was outside the valid range for this operation */
	ERR_BAD_HANDLE,			/* The operation is missing a handler or callback to call */

	/* State errors */
	ERR_BAD_STATE,			/* Operation couldn't complete because it's current state wouldn't allow the operation to complete (eg: pre-conditions not satisfied) */
	ERR_TIMED_OUT,			/* The operation timed-out */
	ERR_CANCELED,			/* The operation has been canceled */
	ERR_NOT_FOUND,			/* A component is missing */
	ERR_ALREADY_EXIST,		/* A given unique-id is already taken */
	ERR_ACCESS_DENIED,		/* The operation couldn't be completed because it's missing privileges */
	ERR_TARGET_BUSY,		/* The target of the operation is busy */
	ERR_NO_MORE_ID,			/* There is no more ID available */

	/* Mapping errors */
	ERR_ALREADY_MAPPED,		/* The given virtual address is already mapped */
	ERR_NOT_MAPPED,			/* The given virtual address is not mapped*/
	ERR_CANT_MAP,			/* The mapping can't be done (reasons may be arch-dependant) */

	/* Filesystem errors */
	ERR_ALREADY_MOUNTED,		/* The given mountpoint is already taken */
	ERR_BAD_DEVICE,			/* The filesystem failed to execute an important device-related operation */
	ERR_NOT_DIRECTORY,		/* The given path isn't a directory */
	ERR_NOT_REGULAR_FILE,		/* The given path isn't a regular file */
};

typedef enum status	status_t;

#endif /* !_CHAOSERR_H_ */

