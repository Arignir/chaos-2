/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/kalloc.h>
#include <kernel/vmm.h>
#include <kernel/mutex.h>
#include <string.h>

/*
** Kernel memory allocator.
** This is NOT suitable for user-space memory allocation.
**
** This is a pretty naive and straightforward implementation.
** It can definitely be improved, but i didn't want to waste
** time doing it. Feel free to improve it :)
*/

static struct mutex kheap_lock = MUTEX_DEFAULT;

/* Malloc's data structures */
static struct block *kalloc_head;
static struct block *kalloc_tail;

static virtaddr_t kernel_heap_start;
static size_t kernel_heap_size;

/*
** Acts a bit like the POSIX sbrk()'s syscall, excepts it can't go downward.
*/
static virtaddr_t
grow_heap(uintptr inc)
{
	virtaddr_t oldbrk;
	virtaddr_t old;
	virtaddr_t new;

	old = (uchar *)kernel_heap_start + kernel_heap_size;
	new = (uchar *)old + inc;
	oldbrk = old;
	if (new > kernel_heap_start)
	{
		old = (virtaddr_t)ROUND_DOWN((uintptr)old, PAGE_SIZE);
		new = (virtaddr_t)ROUND_DOWN((uintptr)new, PAGE_SIZE);
		kernel_heap_size += inc;
		if (new > old) {
			if (unlikely(mmap(
					(uchar *)old + PAGE_SIZE,
					PAGE_ALIGN(inc),
					MMAP_WRITE) == NULL))
			{
				goto err;
			}
			return (oldbrk);
		}
	}
err:
	return ((virtaddr_t)-1);
}

/*
** Looks for a free block that can contain at least the given size.
*/
static struct block *
get_free_block(size_t size)
{
	struct block *block;

	block = kalloc_head;
	while (block <= kalloc_tail)
	{
		if (!block->used && block->size >= size)
			return (block);
		block = (struct block *)((char *)block + sizeof(struct block) + block->size);
	}
	return (NULL);
}

/*
** Split the given block at the given size, if possible.
** Note that size must not be greater that the block size.
*/
static void
split_block(struct block *block, size_t size)
{
	struct block *new;
	struct block *next;

	if (block->size - size > sizeof(struct block) + 1)
	{
		new = (struct block *)((char *)block + sizeof(struct block) + size);
		new->used = false;
		new->size = block->size - size - sizeof(struct block);
		if (kalloc_tail == block) {
			kalloc_tail = new;
		}
		block->size = size;
		new->prev = block;

		next = (struct block *)((char *)new + sizeof(struct block) + new->size);
		if (next <= kalloc_tail) {
			next->prev = new;
		}
	}
}

/*
** Try to join the given block with the next one if they are both free.
*/
static void
join_block(struct block *block)
{
	struct block *other;
	struct block *next;

	other = (struct block *)((char *)block + sizeof(struct block) + block->size);
	if (!block->used && other <= kalloc_tail && !other->used)
	{
		block->size += sizeof(struct block) + other->size;
		if (other == kalloc_tail) {
			kalloc_tail = block;
		}

		next = (struct block *)((char *)other + sizeof(struct block) + other->size);
		if (next <= kalloc_tail) {
			next->prev = block;
		}
	}
}

/*
** malloc(), but using memory in kernel space.
** TODO Make this function safer (overflow)
*/
virtaddr_t
kalloc(size_t size)
{
	struct block *block;

	mutex_acquire(&kheap_lock);
	size += (size == 0);
	size = ALIGN(size, sizeof(void *));
	block = get_free_block(size);
	if (block) {
		block->used = true;
		split_block(block, size);
		goto ret_ok;
	}
	block = grow_heap(sizeof(struct block) + size);
	if (unlikely(block == (void *)-1u)) {
		goto ret_err;
	}
	block->used = true;
	block->size = size;
	block->prev = kalloc_tail;
	kalloc_tail = block;
	if (unlikely(kalloc_head == (void *)1u)) {
		kalloc_head = block;
	}
	if (block->prev) {
		join_block(block->prev);
	}

ret_ok:
	mutex_release(&kheap_lock);
	return ((void *)((char *)block + sizeof(struct block)));

ret_err:
	mutex_release(&kheap_lock);
	return (NULL);
}

/*
** free(), but using memory in kernel space.
** TODO Make this function safer (overflow)
*/
void
kfree(virtaddr_t ptr)
{
	struct block *block;

	if (ptr)
	{
		mutex_acquire(&kheap_lock);
		block = (struct block *)((char *)ptr - sizeof(struct block));
		assert_kalloc(block->used);
		block->used = false;
		join_block(block);
		if (block->prev) {
			join_block(block->prev);
		}
		mutex_release(&kheap_lock);
	}
}

/*
** realloc(), but using memory in kernel space.
** TODO Make this function safer (overflow)
*/
virtaddr_t
krealloc(virtaddr_t old, size_t ns)
{
	void *ptr;
	struct block *block;

	ptr = kalloc(ns);
	if (ptr != NULL && old) {
		block = (struct block *)((char *)old - sizeof(struct block));
		memcpy(ptr, old, block->size > ns ? ns : block->size);
		kfree(old);
	}
	return (ptr);
}

/*
** calloc(), but using memory in kernel space.
** TODO Make this function safer (overflow)
*/
virtaddr_t
kcalloc(size_t a, size_t b)
{
	void *ptr;

	ptr = kalloc(a * b);
	if (likely(ptr != NULL)) {
		memset(ptr, 0, a * b);
	}
	return (ptr);
}

void
kalloc_init(void)
{
	kalloc_head = (void *)1u;
	kalloc_tail = NULL;
	kernel_heap_start = (uchar *)KERNEL_VIRTUAL_END + PAGE_SIZE;
	kernel_heap_size = 0;

	/* kalloc() algorithm need the first page to be mapped. */
	assert_eq(mmap(kernel_heap_start, PAGE_SIZE, MMAP_WRITE), kernel_heap_start);
}
