/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF symbols handling
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

elf_symbols_t *elf_symbols_read_symtab(const Elf64_Ehdr *elf,
	const Elf64_Sym *symtab, uint64_t len)
{
	elf_symbols_t *symbols = malloc(len * sizeof(*symtab));

	if (symbols) {
		for (uint64_t i = 0; i < len; i++) {
			symbols[i].name = elf_strtab_get(elf, false,
				symtab[i].st_name);
			symbols[i].sym = &symtab[i];
		}
	}
	return (symbols);
}

char elf_symbols_get_type(const Elf64_Ehdr *elf, const Elf64_Sym *sym)
{
	char c;
	const char *section;
	const Elf64_Shdr *shdr = elf_section_get(elf, sym->st_shndx);

	if (sym->st_name == 0 || ELF64_ST_TYPE(sym->st_info) == STT_FILE)
		return (0);
	if (ELF64_ST_BIND(sym->st_info) == STB_WEAK) {
		c = sym->st_value ? 'W' : 'w';
		return (c);
	}
	if (sym->st_shndx == SHN_UNDEF)
		return ('U');
	if (sym->st_shndx == SHN_ABS)
		return ('A');
	if (sym->st_shndx == SHN_COMMON)
		return ('C');
	section = elf_strtab_get(elf, true, shdr->sh_name);
	for (const nm_sect2type_map_t *m = SECTION2TYPE_MAP; m->section; m++) {
		if (strncmp(m->section, section, strlen(m->section)) == 0) {
			c = ELF64_ST_BIND(sym->st_info) == STB_GLOBAL ?
				toupper(m->type) : m->type;
			return (c);
		}
	}
	return ('?');
}

int elf_symbols_sorter(const void *a, const void *b)
{
	const char *a_name = ((elf_symbols_t *)a)->name;
	const char *b_name = ((elf_symbols_t *)b)->name;

	while (*a_name && *b_name &&
		(toupper(*a_name) == toupper(*b_name) ||
		!isalnum(*a_name) || !isalnum(*b_name))) {
		if (toupper(*a_name) == toupper(*b_name)) {
			a_name++;
			b_name++;
		}
		if (*a_name && !isalnum(*a_name))
			a_name++;
		if (*b_name && !isalnum(*b_name))
			b_name++;
	}
	return (toupper(*a_name) - toupper(*b_name));
}
