/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF symbol table handling
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "nmobjdump.h"

elf_sym_t *elf_symtab(elf_t *elf, Elf64_Sym *symtab_data, uint64_t length)
{
	elf_sym_t *symtab = malloc(length * sizeof(*symtab));

	if (symtab) {
		for (uint64_t i = 0; i < length; i++) {
			symtab[i].name = elf_strtab_get(elf, false,
				symtab_data[i].st_name);
			symtab[i].sym = &symtab_data[i];
		}
	}
	return (symtab);
}

int elf_symtab_sort(const void *a, const void *b)
{
	const char *a_name = ((elf_sym_t *)a)->name;
	const char *b_name = ((elf_sym_t *)b)->name;

	while (*a_name && *b_name &&
		(toupper(*a_name) == toupper(*b_name) ||
		!isalnum(*a_name) || !isalnum(*b_name))) {
		if (toupper(*a_name) == toupper(*b_name)) {
			a_name++;
			b_name++;
		}
		if (!isalnum(*a_name))
			a_name++;
		if (!isalnum(*b_name))
			b_name++;
	}
	return (toupper(*a_name) - toupper(*b_name));
}
