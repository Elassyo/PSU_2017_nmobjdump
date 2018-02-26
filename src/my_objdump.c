/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** my_objdump implementation
*/

#include <stdio.h>
#include "my_objdump.h"

static int my_objdump_elf(file_t const *file)
{
	elf_t *elf;

	elf = elf_file_open(file);
	if (!elf) {
		fprintf(stderr, "my_objdump: %s: %s\n", file->f_path,
			errno == EIO ? "File truncated" : "Invalid file");
		return (84);
	}
	printf("\n%s:     file format %s-%s\n", file->f_path,
		elf_info_format(elf), elf_info_machine(elf));
	my_objdump_file_header(elf);
	putchar('\n');
	my_objdump_full_content(elf);
	return (0);
}

static int my_objdump_file(file_t const *file)
{
	int ret = 0;
	ar_file_t *ar_file;

	if (elf_file_check(file)) {
		ret = my_objdump_elf(file);
	} else if (ar_file_check(file)) {
		printf("In archive %s:\n", file->f_path);
		ar_file = ar_file_iterate(file, NULL);
		while (ar_file) {
			ret |= my_objdump_file(&ar_file->af_file);
			ar_file = ar_file_iterate(file, ar_file);
		}
	} else {
		fprintf(stderr, "my_objdump: %s: File format not recognized\n",
			file->f_path);
	}
	return (ret);
}

static int my_objdump(char const *path)
{
	int ret;
	file_t *file = fs_open(path);

	if (!file) {
		fprintf(stderr, "my_objdump: '%s': %m\n", path);
		return (84);
	}
	ret = my_objdump_file(file);
	fs_close(file);
	return (ret);
}

int main(int argc, char const *const *argv)
{
	int ret = 0;

	if (argc == 1)
		ret = my_objdump("a.out");
	else if (argc == 2)
		ret = my_objdump(argv[1]);
	else {
		for (int i = 1; i < argc; i++)
			ret |= my_objdump(argv[i]);
	}
	return (ret);
}
