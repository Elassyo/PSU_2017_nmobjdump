/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** my_nm implementation
*/

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_nm.h"

static void my_nm_print_symtab(elf_t const *elf)
{
	elf_symbol_t *symbols = malloc(elf->e_symnum * sizeof(*symbols));
	char buf[20];
	char type;

	assert(symbols != NULL);
	memcpy(symbols, elf->e_symbols, elf->e_symnum * sizeof(*symbols));
	qsort(symbols, elf->e_symnum, sizeof(*elf->e_symbols),
		elf_symbol_cmp);
	for (uint64_t i = 0; i < elf->e_symnum; i++) {
		type = my_nm_symbol_type(elf, &symbols[i]);
		if (type != 'U' && type != 'w')
			snprintf(buf, 20, "%0*lx", elf->e_addrsz / 4,
				symbols[i].sym_value);
		else
			buf[0] = 0;
		if (type == 0)
			continue;
		printf("%*s %c %s\n", elf->e_addrsz / 4, buf, type,
			symbols[i].sym_name);
	}
	free(symbols);
}

static int my_nm_elf(file_t const *file)
{
	elf_t *elf;

	elf = elf_file_open(file);
	if (!elf) {
		fprintf(stderr, "my_nm: %s: %s\n", file->f_path, errno == EIO ?
			"File truncated" : "Invalid file"
		);
		return (84);
	}
	if (!elf->e_symbols)
		fprintf(stderr, "my_nm: %s: no symbols\n", file->f_path);
	else
		my_nm_print_symtab(elf);
	elf_file_close(elf);
	return (0);
}

static int my_nm_file(file_t const *file, bool print_path)
{
	int ret = 0;
	ar_file_t *ar_file;

	if (elf_file_check(file)) {
		if (print_path)
			printf("\n%s:\n", file->f_path);
		ret = my_nm_elf(file);
	} else if (ar_file_check(file)) {
		if (print_path)
			printf("\n%s:\n", file->f_path);
		ar_file = ar_file_iterate(file, NULL);
		while (ar_file) {
			ret |= my_nm_file(&ar_file->af_file, true);
			ar_file = ar_file_iterate(file, ar_file);
		}
	} else {
		fprintf(stderr, "my_nm: %s: File format not recognized\n",
			file->f_path);
	}
	return (ret);
}

static int my_nm(char const *path, bool print_path)
{
	int ret;
	file_t *file = fs_open(path);

	if (!file) {
		fprintf(stderr, "my_nm: '%s': %m\n", path);
		return (84);
	}
	ret = my_nm_file(file, print_path);
	fs_close(file);
	return (ret);
}

int main(int argc, char const *const *argv)
{
	int ret = 0;

	if (argc == 1)
		ret = my_nm("a.out", false);
	else if (argc == 2)
		ret = my_nm(argv[1], false);
	else {
		for (int i = 1; i < argc; i++)
			ret |= my_nm(argv[i], true);
	}
	return (ret);
}
