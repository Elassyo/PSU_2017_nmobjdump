/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** my_objdump implementation
*/

#include <stdio.h>
#include "my_objdump.h"

static int my_objdump_parse_args(int argc, char const *const *argv,
	objdump_options_t *opt)
{
	int i;

	for (i = 1; i < argc && argv[i][0] == '-'; i++) {
		if (argv[i][1] == 'f')
			opt->file_header = true;
		else if (argv[i][1] == 's')
			opt->full_content = true;
		else {
			fprintf(stderr, "my_objdump: invalid switch '%c'\n",
				argv[i][0]);
			return (-1);
		}
	}
	return (i);
}

static int my_objdump_elf(file_t const *file, objdump_options_t const *opt)
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
	if (opt->file_header)
		my_objdump_file_header(elf);
	putchar('\n');
	if (opt->full_content)
		my_objdump_full_content(elf);
	return (0);
}

static int my_objdump_file(file_t const *file, objdump_options_t const *opt)
{
	int ret = 0;
	ar_file_t *ar_file;

	if (elf_file_check(file)) {
		ret = my_objdump_elf(file, opt);
	} else if (ar_file_check(file)) {
		printf("In archive %s:\n", file->f_path);
		ar_file = ar_file_iterate(file, NULL);
		while (ar_file) {
			ret |= my_objdump_file(&ar_file->af_file, opt);
			ar_file = ar_file_iterate(file, ar_file);
		}
	} else {
		fprintf(stderr, "my_objdump: %s: File format not recognized\n",
			file->f_path);
	}
	return (ret);
}

static int my_objdump(char const *path, objdump_options_t const *opt)
{
	int ret;
	file_t *file = fs_open(path);

	if (!file) {
		fprintf(stderr, "my_objdump: '%s': %m\n", path);
		return (84);
	}
	ret = my_objdump_file(file, opt);
	fs_close(file);
	return (ret);
}

int main(int argc, char const *const *argv)
{
	int i;
	int ret = 0;
	objdump_options_t opt = { false, false };

	i = my_objdump_parse_args(argc, argv, &opt);
	if (i == -1)
		return (84);
	if (!(opt.file_header || opt.full_content)) {
		fprintf(stderr, "my_objdump: no mode specified\n");
		return (84);
	}
	if (argc - i == 0)
		ret = my_objdump("a.out", &opt);
	else if (argc - i == 1)
		ret = my_objdump(argv[i], &opt);
	else {
		for (; i < argc; i++)
			ret |= my_objdump(argv[i], &opt);
	}
	return (ret);
}
