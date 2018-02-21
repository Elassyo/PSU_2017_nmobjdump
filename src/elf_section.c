/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF section handling
*/

#include <string.h>
#include "nmobjdump.h"

Elf64_Shdr const *elf_section_find(Elf64_Ehdr const *elf, char const *name)
{
	Elf64_Shdr *shdrtab = (void *)elf + elf->e_shoff;

	if (!elf_strtab_get(elf, true, 0))
		return (NULL);
	for (uint16_t i = 0; i < elf->e_shnum; i++) {
		if (strcmp(elf_strtab_get(elf, true, shdrtab[i].sh_name),
			name) == 0)
			return (shdrtab + i);
	}
	return (NULL);
}

Elf64_Shdr const *elf_section_get(Elf64_Ehdr const *elf, Elf64_Section idx)
{
	Elf64_Shdr *shdrtab = (void *)elf + elf->e_shoff;

	if (idx >= elf->e_shnum)
		return (NULL);
	return (shdrtab + idx);
}
