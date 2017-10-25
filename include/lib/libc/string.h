/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _LIBC_STRING_H_
# define _LIBC_STRING_H_

# include <chaosdef.h>

/*
** Will be completed when needed.
*/

size_t		strlen(char const *) __pure;
char		*strdup(char const *str);
char 		*strcpy(char *, char const *);
char 		*strncpy(char *, char const *, size_t);
char		*strstr(char const *haystack, char const *needle) __pure;
char		*strcat(char *dest, char const *src);
int		strcmp(char const *s1, char const *s2) __pure;
int		strncmp(char const *s1, char const *s2, size_t n) __pure;

void		*memset(void *src, int c, size_t n);
void		*memcpy(void *dest, void const *src, size_t n);
int		memcmp(void const *s1, void const *s2, size_t n) __pure;
void		*memchr(void const *src, int c, size_t n) __pure;
void 		*memmove(void *dest, void const *src, size_t n);

#endif /* !_LIBC_STRING_H_ */
