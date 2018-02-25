/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF sections handling
*/

#include <string.h>
#include "my_elf.h"

elf_section_t const *elf_section_find(elf_t const *elf,
	char const *name, unsigned int type)
{
	elf_section_t const *type_match = NULL;

	for (size_t i = 0; i < elf->e_sectnum; i++) {
		if (strcmp(elf->e_sections[i].s_name, name) == 0)
			return (&elf->e_sections[i]);
		if (elf->e_sections[i].s_type == type && !type_match)
			type_match = &elf->e_sections[i];
	}
	return (type_match);
}
