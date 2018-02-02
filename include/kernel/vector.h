/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_VECTOR_H_
# define _KERNEL_VECTOR_H_

# include <chaosdef.h>
# include <chaoserr.h>
# include <kernel/kalloc.h>
# include <string.h>

# if KCONFIG_DEBUG_VECTOR
#  define assert_vector(x) assert(x)
# else
#  define assert_vector(x)
# endif /* KCONFIG_DEBUG_VECTOR */

# define NEW_VECTOR(type) \
	type *_data; \
	size_t _capacity; \
	size_t _length;

# define vector_init(x) \
	(x)->_data = NULL; \
	(x)->_capacity = 0; \
	(x)->_length = 0;

# define vector_clear(x) \
	kfree((x)->_data); \
	(x)->_data = NULL; \
	(x)->_capacity = 0; \
	(x)->_length = 0;

# define vector_pushfront(x, y) vector_insert(x, 0, y)
# define vector_pushback(x, y) vector_insert(x, (x)->_length, y)

# define vector_insert(x, y, z) \
	({ \
		size_t _i = (y); \
		status_t _ret = OK; \
		assert_vector(_i <= (x)->_length); \
		if ((x)->_capacity == (x)->_length) {\
			_ret = vector_resize(x, (x)->_capacity * 2 + 1); \
		} \
		if (likely(!_ret)) { \
			memmove( \
				(x)->_data + _i + 1, \
				(x)->_data + _i, \
				((x)->_length - _i) * sizeof(*(x)->_data) \
			); \
			(x)->_data[_i] = (z); \
			++(x)->_length; \
		} \
		_ret; \
	})

# define vector_popfront(x) vector_remove(x, 0)
# define vector_popback(x) vector_remove(x, (x)->_length - 1)

# define vector_remove(x, idx) \
	({ \
		size_t _i = (idx); \
		assert_vector(_i < (x)->_length); \
		memmove( \
			(x)->_data + _i, \
			(x)->_data + _i + 1, \
			((x)->_length - _i - 1) * sizeof(*(x)->_data) \
		);\
		--(x)->_length; \
		if ((x)->_length <= (x)->_capacity / 4) { \
			vector_resize(x, (x)->_capacity / 2); \
		} \
	})

# define vector_remove_value(x, val) \
	({ \
		typeof(val) _val = val; \
		vector_foreach_count(x, _e, _i) { \
			if (*_e == _val) { \
				vector_remove(x, _i); \
				break; \
			} \
		} \
	})

# define vector_resize(x, y) \
	({ \
		typeof((x)->_data) tmp;\
		status_t _ret = ERR_NO_MEMORY; \
		size_t _new = (y); \
		tmp = krealloc((x)->_data, sizeof(*(x)->_data) * _new); \
		if (tmp) { \
			_ret = OK; \
			(x)->_capacity = _new; \
			(x)->_data = tmp; \
		} \
		_ret; \
	})

# define vector_length(x) (x)->_length

# define vector_at(x, i) \
	({ \
		size_t _i = (i); \
		assert_vector(_i < (x)->_length); \
		(x)->_data + (_i); \
	})

# define vector_foreach(x, name) \
	for (typeof((x)->_data) name = (x)->_data; name < (x)->_data + (x)->_length; ++(name))

# define vector_foreach_count(x, name, count) \
	size_t count; \
	typeof((x)->_data) name; \
	for (count = 0, name = (x)->_data; count < (x)->_length; ++name, ++count)

#endif /* !_KERNEL_VECTOR_H_ */
