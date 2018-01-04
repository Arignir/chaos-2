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
**
** kalloc_aligned() is using a dirty trick to easily make aligned allocations.
** It calls malloc() with a size of ('size' + PAGE_SIZE) bytes and align the returned
** pointer to a page boundary. Then it stores the old pointer in aladdr_array so that
** free knows it.
**
** This definitely deserve an improvement, but eh, it's not fun enough.
** (I hate writting mallocs(). I mean it.)
*/

/* Mutex protecting concurrent allocations */
static struct mutex kheap_lock = MUTEX_DEFAULT;

/* kalloc() global variables */
static struct block *kalloc_head;
static struct block *kalloc_tail;

/* grow_heap() global variables */
static virtaddr_t kernel_heap_start = (uchar *)KERNEL_VIRTUAL_END + PAGE_SIZE;
static size_t kernel_heap_size;

/* mutex protecting concurrent access to the aligned array */
static struct mutex aladdr_mutex = MUTEX_DEFAULT;

/* kalloc_aligned() global variables */
static struct aligned_address *aladdr_array = NULL;
static size_t aladdr_size = 0;

/*
** Grows kernel heap by 'inc' bytes, performing the allocation using mmap().
*/
static virtaddr_t
grow_heap(uintptr inc)
{
	virtaddr_t oldbrk;
	virtaddr_t old;
	virtaddr_t new;
	uintptr round_add;

	old = (uchar *)kernel_heap_start + kernel_heap_size;
	new = (uchar *)old + inc;
	oldbrk = old;
	if (likely(new > kernel_heap_start))
	{
		new = (virtaddr_t)ROUND_DOWN((uintptr)new, PAGE_SIZE);
		old = (virtaddr_t)ROUND_DOWN((uintptr)old, PAGE_SIZE);
		round_add = (uintptr)new - (uintptr)old;
		kernel_heap_size += inc;
		if (new > old) {
			if (unlikely(mmap(
				(uchar *)old + PAGE_SIZE,
				round_add,
				MMAP_WRITE) == NULL))
			{
				kernel_heap_size -= inc;
				return ((virtaddr_t)-1);
			}
#if KCONFIG_DEBUG_KALLOC
			memset((uchar *)old + PAGE_SIZE, 42, round_add);
#endif /* KCONFIG_DEBUG_KALLOC */
		}
		return (oldbrk);
	}
	panic("Tried to go beyond minimum kernel heap");
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
		block = (struct block *)((uchar *)(block + 1) + block->size);
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
		new = (struct block *)((uchar *)(block + 1) + size);
		new->used = false;
		new->size = block->size - size - sizeof(struct block);
		if (kalloc_tail == block) {
			kalloc_tail = new;
		}
		block->size = size;
		new->prev = block;
		next = (struct block *)((uchar *)(new + 1) + new->size);
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

	other = (struct block *)((uchar *)(block + 1) + block->size);
	if (!block->used && other <= kalloc_tail && !other->used)
	{
		block->size += sizeof(struct block) + other->size;
		if (other == kalloc_tail) {
			kalloc_tail = block;
		}

		next = (struct block *)((uchar *)(other + 1) + other->size);
		if (next <= kalloc_tail) {
			next->prev = block;
		}
	}
}

/*
** Given an aligned virtual address returned from kmalloc(), returns the original
** virtual address so it can be passed to free().
** Go trough aligned_addresses_ptr, finds a ptr matching the given one and remove the
** entry from the array (swapping with the last entry and using kalloc()), and
** then return the original pointer.
**
** This is part of a trick to easily
** have an aligned malloc() without having to lose to much time changing the
** current implementation.
*/
static virtaddr_t
get_original_and_remove(virtaddr_t ptr)
{
	struct aligned_address *aa;
	struct aligned_address *new_aladdr_array;

	mutex_acquire(&aladdr_mutex);
	aa = aladdr_array;
	while (aa < aladdr_array + aladdr_size) {
		if (aa->ptr == ptr) {
			ptr = aa->ori_ptr;
			aladdr_size -= 1;
			*aa = aladdr_array[aladdr_size];
			new_aladdr_array = krealloc(
				aladdr_array,
				sizeof(*aladdr_array) * aladdr_size
			);
			if (new_aladdr_array != NULL) {
				aladdr_array = new_aladdr_array;
			}
			break;
		}
		++aa;
	}
	mutex_release(&aladdr_mutex);
	return (ptr);
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

ret_ok:
	mutex_release(&kheap_lock);
	return (block + 1);

ret_err:
	mutex_release(&kheap_lock);
	return (NULL);
}

/*
** kalloc(), but using memory in kernel space and returning a page-aligned pointer.
** TODO Make this function safer (overflow)
*/
virtaddr_t
kalloc_aligned(size_t size)
{
	virtaddr_t ptr;
	virtaddr_t aligned_ptr;
	virtaddr_t new_aladdr_array;

	ptr = kalloc(size + PAGE_SIZE);
	aligned_ptr = (virtaddr_t)PAGE_ALIGN((uintptr)ptr);
	if (ptr && ptr != aligned_ptr) {
		mutex_acquire(&aladdr_mutex);
		new_aladdr_array  = krealloc(
			aladdr_array,
			sizeof(*aladdr_array) * (aladdr_size + 1)
		);
		if (unlikely(new_aladdr_array == NULL)) {
			mutex_release(&aladdr_mutex);
			kfree(ptr);
			return (NULL);
		}
		aladdr_array = new_aladdr_array;
		aladdr_array[aladdr_size] = (struct aligned_address) {
			.ptr = aligned_ptr,
			.ori_ptr = ptr,
		};
		aladdr_size += 1;
		mutex_release(&aladdr_mutex);
	}
	assert_kalloc(IS_PAGE_ALIGNED(aligned_ptr));
	return (aligned_ptr);
}

/*
** Like kalloc_aligned(), but using the given virtual pages as the content
**
** size must be page-aligned, or undefined behaviour may occur.
*/
virtaddr_t
kalloc_device(size_t size, physaddr_t pa)
{
	virtaddr_t ptr;

	assert_kalloc(IS_PAGE_ALIGNED(size));
	ptr = kalloc_aligned(size);
	if (likely(ptr != NULL
			&& mmap_device(ptr, pa, size, MMAP_WRITE | MMAP_REMAP) == ptr))
	{
		return (ptr);
	}
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
		if (IS_PAGE_ALIGNED(ptr)) {
			ptr = get_original_and_remove(ptr);
		}
		mutex_acquire(&kheap_lock);
		block = (struct block *)ptr - 1;
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
	aladdr_array = NULL;
	aladdr_size = 0;

	/* kalloc() algorithm need the first page to be mapped. */
	assert_eq(mmap(kernel_heap_start, PAGE_SIZE, MMAP_WRITE), kernel_heap_start);
}
