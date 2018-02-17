/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** my_nm implementation
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "nmobjdump.h"

static void my_nm_print_symtab(const Elf64_Ehdr *elf,
	const Elf64_Sym *symtab, uint64_t len)
{
	char buf[17];
	char type;
	elf_symbols_t *symbols = elf_symbols_read_symtab(elf, symtab, len);

	qsort(symbols, len, sizeof(*symbols), elf_symbols_sorter);
	for (uint64_t i = 0; i < len; i++) {
		type = elf_symbols_get_type(elf, symbols[i].sym);
		if (type != 'U' && type != 'w')
			snprintf(buf, 17, "%016lx", symbols[i].sym->st_value);
		else
			buf[0] = 0;
		if (type == 0)
			continue;
		printf("%16s %c %s\n", buf, type, symbols[i].name);
	}
	free(symbols);
}

static int my_nm_elf(file_t *file)
{
	const Elf64_Ehdr *elf = file->f_data;
	const Elf64_Shdr *symtab_shdr;

	if (elf->e_shoff == 0 || elf->e_shoff > file->f_size ||
		elf->e_shnum == 0) {
		fprintf(stderr, "my_nm: %s: no sections\n", file->f_path);
		return (84);
	}
	symtab_shdr = elf_section_find(elf, ".symtab");
	if (symtab_shdr && symtab_shdr->sh_offset < file->f_size &&
		symtab_shdr->sh_size != 0 && symtab_shdr->sh_entsize != 0)
		my_nm_print_symtab(elf, (void *)elf + symtab_shdr->sh_offset,
			symtab_shdr->sh_size / symtab_shdr->sh_entsize);
	else
		fprintf(stderr, "my_nm: %s: no symbols\n", file->f_path);
	return (0);
}

static int my_nm_file(file_t *file, bool print_path)
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

static int my_nm(const char *path, bool print_path)
{
	int ret;
	file_t *file = fs_open("my_nm", path);

	if (!file)
		return (84);
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
