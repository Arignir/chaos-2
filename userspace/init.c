/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <ulib.h>
#include <arch/x86/asm.h>

# define CMD_LEN	512

static void do_ls(char *);
static void do_cat(char *);
static void do_clear(char *);

struct command
{
	char const *name;
	bool builtin;
	union
	{
		void (*func)(char *);
		char const *path;
	};
};

static struct command commands[] =
{
	{
		.name = "ls",
		.builtin = true,
		.func = &do_ls,
	},
	{
		.name = "cat",
		.builtin = true,
		.func = &do_cat,
	},
	{
		.name = "clear",
		.builtin = true,
		.func = &do_clear,
	},
	{
		.name = NULL,
		.builtin = true,
		.func = NULL,
	},
};

static void
do_ls(char *arg)
{
	struct dirent dirent;
	dir_handle_t handle;

	handle = opendir(arg);
	if (handle < 0) {
		putstr("opendir() failed\n");
		return;
	}
	while (readdir(handle, &dirent) == OK) {
		printf("%s\t%s\n",
			dirent.dir ? "d" : "f",
			dirent.name
		);
	}
	closedir(handle);
}

static void
do_cat(char *arg)
{
	char buff[PAGE_SIZE];
	int out;
	file_handle_t handle;

	handle = open(arg);
	if (handle < 0) {
		printf("%s: file not found\n", arg);
		return;
	}

	do {
		out = read(handle, buff, PAGE_SIZE);
		if (out < 0) {
			printf("read() failed\n");
		} else {
			write(1, buff, out);
		}
	}
	while (out > 0);
	close(handle);
}

static void
do_clear(char *arg __unused)
{
	char str[26];
	for (int i = 0; i < 25; ++i) {
		str[i] = '\n';
	}
	write(1, str, 25);
}

static char *
next_word(char *old)
{
	char *word;

	while (*old && is_whitespace(*old))
		++old;
	word = old;
	while (*old && !is_whitespace(*old))
		++old;
	*old = 0;
	return (word);
}

static void
read_cmd(char *cmd, char **arg0_ptr, char **arg1_ptr)
{
	size_t size;
	char *arg0;
	char *arg1;

	size = 0;
	printf("chaosh$ ");
	while (42) {
		char c = keyboard();
		if (c == '\b') {
			if (size > 0) {
				putc('\b');
				putc(' ');
				putc('\b');
				cmd[size--] = '\0';
			}
		} else if (c != '\n') {
			putc(c);
			cmd[size++] = c;
		}
		if (c == '\n' || size >= CMD_LEN - 1) {
			cmd[size] = 0;
			putc('\n');
			break;
		}
	}
	arg0 = next_word(cmd);
	arg1 = arg0 + strlen(arg0);
	if (arg1 < cmd + size) {
		arg1 += 1;
	}
	arg1 = next_word(arg1);
	*arg0_ptr = arg0;
	*arg1_ptr = arg1;
}

static void
exec_cmd(char *arg0, char *arg1)
{
	struct command *cmd;

	for (cmd = commands; cmd->name; ++cmd) {
		if (!strcmp(cmd->name, arg0)) {
			if (cmd->builtin) {
				cmd->func(arg1);
			} else {
				exec(cmd->path);
				printf("%s: exec failed\n", arg0);
			}
			return ;
		}
	}
	printf("%s: command not found\n", arg0);
}

__optimize(0)
int main(void) {
	char cmd[CMD_LEN];
	char *arg0;
	char *arg1;

	putstr("Hello Userspace World!\n");
	while (42) {
		read_cmd(cmd, &arg0, &arg1);
		exec_cmd(arg0, arg1);
	}
	exit(1);
}
