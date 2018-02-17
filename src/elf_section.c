/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF section handling
*/

#include <string.h>
#include "nmobjdump.h"

const Elf64_Shdr *elf_section_find(const Elf64_Ehdr *elf, const char *name)
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

const Elf64_Shdr *elf_section_get(const Elf64_Ehdr *elf, Elf64_Section idx)
{
	Elf64_Shdr *shdrtab = (void *)elf + elf->e_shoff;

	if (idx >= elf->e_shnum)
		return (NULL);
	return (shdrtab + idx);
}
