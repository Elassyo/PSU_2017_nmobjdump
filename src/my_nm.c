/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** my_nm main function
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "nmobjdump.h"

static const nm_sect2type_map_t SECTION2TYPE_MAP[] = {
	{ ".bss", 'b' },
	{ ".data", 'd' },
	{ ".dynamic", 'd' },
	{ ".eh_frame", 'r' },
	{ ".fini", 't' },
	{ ".got", 'd' },
	{ ".init", 't' },
	{ ".rdata", 'r' },
	{ ".rodata", 'r' },
	{ ".sbss", 's' },
	{ ".scommon", 'c' },
	{ ".sdata", 'g' },
	{ ".tbss", 'b' },
	{ ".tdata", 'd' },
	{ ".text", 't' },
	{ 0, 0 }
};

static char my_nm_get_symbol_type(elf_t *elf, Elf64_Sym *sym)
{
	Elf64_Shdr *shdr = elf_section_get(elf,	sym->st_shndx);
	const char *section;

	if (sym->st_name == 0 ||
		ELF64_ST_TYPE(sym->st_info) == STT_SECTION ||
		ELF64_ST_TYPE(sym->st_info) == STT_FILE)
		return (0);
	if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
		return (sym->st_value ? 'W' : 'w');
	if (sym->st_value == 0)
		return ('U');
	section = elf_strtab_get(elf, true, shdr->sh_name);
	for (const nm_sect2type_map_t *m = SECTION2TYPE_MAP; m->section; m++) {
		if (strncmp(m->section, section, strlen(m->section)) == 0)
			return (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL ?
				toupper(m->type) : m->type);
	}
	return ('?');
}

static void my_nm_print_symtab(elf_t *elf,
	Elf64_Sym *symtab_data, uint64_t length)
{
	char buf[17];
	char type;
	elf_sym_t *symtab = elf_symtab(elf, symtab_data, length);

	qsort(symtab, length, sizeof(*symtab), elf_symtab_sort);
	for (uint64_t i = 0; i < length; i++) {
		if (symtab[i].sym->st_value)
			snprintf(buf, 17, "%016lx", symtab[i].sym->st_value);
		else
			buf[0] = 0;
		type = my_nm_get_symbol_type(elf, symtab[i].sym);
		if (type == 0)
			continue;
		printf("%16s %c %s\n", buf, type, symtab[i].name);
	}
	free(symtab);
}

static int my_nm(const char *path)
{
	elf_t elf;
	Elf64_Shdr *symtab_shdr;

	if (elf_open("my_nm", path, &elf))
		return (84);
	if (elf.buf->e_shoff == 0 || elf.buf->e_shoff > elf.size) {
		fprintf(stderr, "my_nm: %s: no section table in ELF\n", path);
		return (84);
	}
	symtab_shdr = elf_section_find(&elf, ".symtab");
	if (symtab_shdr && symtab_shdr->sh_offset < elf.size &&
		symtab_shdr->sh_size != 0 && symtab_shdr->sh_entsize != 0)
		my_nm_print_symtab(&elf,
			((void *)elf.buf + symtab_shdr->sh_offset),
			symtab_shdr->sh_size / symtab_shdr->sh_entsize);
	else
		fprintf(stderr, "my_nm: %s: no symbols\n", path);
	elf_close(&elf);
	return (0);
}

int main(int argc, char const *const *argv)
{
	int ret = 0;

	if (argc == 1)
		ret = my_nm("a.out");
	else if (argc == 2)
		ret = my_nm(argv[1]);
	else {
		for (int i = 1; i < argc; i++) {
			printf("\n%s:\n", argv[i]);
			ret |= my_nm(argv[i]);
		}
	}
	return (ret);
}
