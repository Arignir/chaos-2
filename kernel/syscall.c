/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/syscall.h>
#include <kernel/thread.h>
#include <kernel/vaspace.h>
#include <kernel/vmm.h>
#include <kernel/fs.h>
#include <drivers/keyboard.h>
#include <drivers/vga.h>

/*
** Creates a new thread starting at the given function.
*/
status_t
sys_clone(void *main)
{
	status_t s;

	s = thread_clone(main);
	return (s);
}

/*
** Exits the current thread
*/
__noreturn
void
sys_exit(uchar status)
{
	current_thread_acquire_write();
	current_vaspace_acquire_write();
	thread_exit(status);
}

/*
** Executes the given binary, replacing the current vaspace by a new one.
*/
status_t
sys_exec(char const *path)
{
	status_t s;

	current_thread_acquire_write();
	current_vaspace_acquire_write();
	if (vmm_validate_str(path, NULL)) {
		thread_exit(EXIT_PAGEFAULT);
	}
	s = thread_exec(path);
	current_vaspace_release_write();
	current_thread_release_write();
	return (s);
}

/*
** Opens the given file.
** Returns a handler descriptor to it or a negative number on error.
*/
file_handle_t
sys_open(char const *path)
{
	struct thread *t;
	file_handle_t handle_desc;
	status_t s;
	struct file_handle **handle;

	t = current_thread();
	current_thread_acquire_write();
	current_vaspace_acquire_write();
	if (vmm_validate_str(path, NULL)) {
		thread_exit(EXIT_PAGEFAULT);
	}
	current_vaspace_release_write();
	handle_desc = thread_find_free_handle(t);
	if (handle_desc < 0)
		goto end;
	handle = (struct file_handle **)vector_at(&t->handles, handle_desc);
	s = fs_open(path, handle);
	if (s) {
		handle_desc = s;
		*handle = NULL;
	}
end:
	current_thread_release_write();
	return (handle_desc);
}

/*
** Reads the given file and put its content in the given buffer.
*/
ssize_t
sys_read(file_handle_t handle, char *buff, size_t size)
{
	size_t out;
	struct thread *t;
	void **handle_ptr;
	status_t s;

	s  = ERR_BAD_HANDLER;
	t = current_thread();
	current_thread_acquire_write();
	current_vaspace_acquire_write();
	if (vmm_validate_buffer((uchar const *)buff, s)) {
		thread_exit(EXIT_PAGEFAULT);
	}
	current_vaspace_release_write();
	if (handle < 0 || (size_t)handle >= vector_length(&t->handles)) {
		goto end;
	}
	handle_ptr = vector_at(&t->handles, handle);
	if (!*handle_ptr) {
		goto end;
	}
	out = size;
	if (!(s = fs_read(*handle_ptr, buff, &out))) {
		s = out;
	}
end:
	current_thread_release_write();
	return (s);
}

/*
** Writes the given buffer to the given file.
*/
ssize_t
sys_write(file_handle_t handle __unused /* TODO */, char const *buff, size_t s)
{
	int r;

	current_thread_acquire_write();
	current_vaspace_acquire_write();
	if (vmm_validate_buffer((uchar const *)buff, s)) {
		thread_exit(EXIT_PAGEFAULT);
	}
	r = vga_putsn(buff, s);
	current_vaspace_release_write();
	current_thread_release_write();
	return (r);
}

/*
** Closes the given file.
*/
status_t
sys_close(file_handle_t handle)
{
	struct thread *t;
	void **handle_ptr;
	status_t s;

	s  = ERR_BAD_HANDLER;
	t = current_thread();
	current_thread_acquire_write();
	if (handle < 0 || (size_t)handle >= vector_length(&t->handles)) {
		goto end;
	}
	handle_ptr = vector_at(&t->handles, handle);
	if (!*handle_ptr) {
		goto end;
	}
	s = fs_close(*handle_ptr);
	*handle_ptr = NULL;
end:
	current_thread_release_write();
	return (s);
}

/*
** Opens the given directory.
** Returns a handle to the given directory.
*/
dir_handle_t
sys_opendir(char const *path)
{
	status_t s;
	struct thread *t;
	dir_handle_t dir_handle_desc;
	struct file_handle *file_handle;
	struct dir_handle **dir_handle;

	t = current_thread();
	current_thread_acquire_write();
	current_vaspace_acquire_write();
	if (vmm_validate_str(path, NULL)) {
		thread_exit(EXIT_PAGEFAULT);
	}
	current_vaspace_release_write();
	dir_handle_desc = thread_find_free_handle(t);
	if (dir_handle_desc < 0)
		goto end;
	dir_handle = (struct dir_handle **)vector_at(&t->handles, dir_handle_desc);
	s = fs_open(path, &file_handle);
	if (s) {
		dir_handle_desc = s;
		*dir_handle = NULL;
		goto end;
	}
	s = fs_opendir(file_handle, dir_handle);
	if (s) {
		dir_handle_desc = s;
		*dir_handle = NULL;
		fs_close(file_handle);
	}
end:
	current_thread_release_write();
	return (dir_handle_desc);
}

/*
** Reads the next entry of the given directory and puts it 'dirent'.
*/
status_t
sys_readdir(dir_handle_t handle, struct dirent *dirent)
{
	struct thread *t;
	void **handle_ptr;
	status_t s;

	s  = ERR_BAD_HANDLER;
	t = current_thread();
	current_thread_acquire_write();
	current_vaspace_acquire_write();
	if (vmm_validate_buffer(dirent, sizeof(*dirent))) {
		thread_exit(EXIT_PAGEFAULT);
	}
	current_vaspace_release_write();
	if (handle < 0 || (size_t)handle >= vector_length(&t->handles)) {
		goto end;
	}
	handle_ptr = vector_at(&t->handles, handle);
	if (!*handle_ptr) {
		goto end;
	}
	s = fs_readdir(*handle_ptr, dirent);
end:
	current_thread_release_write();
	return (s);
}

/*
** Closes the given directory.
*/
status_t
sys_closedir(dir_handle_t handle)
{
	struct thread *t;
	void **handle_ptr;
	status_t s;

	s  = ERR_BAD_HANDLER;
	t = current_thread();
	current_thread_acquire_write();
	if (handle < 0 || (size_t)handle >= vector_length(&t->handles)) {
		goto end;
	}
	handle_ptr = vector_at(&t->handles, handle);
	if (!*handle_ptr) {
		goto end;
	}
	s = fs_closedir(*handle_ptr);
	*handle_ptr = NULL;
end:
	current_thread_release_write();
	return (s);
}

/*
** Blocks until the next keyboard input, and returns in a form of a friendly c-like char.
*/
char
sys_keyboard(void)
{
	return (keyboard_next_input());
}
