/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF strings handling
*/

#include "nmobjdump.h"

char const *elf_strtab_get(Elf64_Ehdr const *elf, bool section_name,
	uint32_t idx)
{
	Elf64_Shdr const *strtab_shdr;

	if (section_name) {
		if (elf->e_shstrndx == SHN_UNDEF ||
			elf->e_shstrndx >= elf->e_shnum)
			return (NULL);
		strtab_shdr = (Elf64_Shdr *)((void *)elf + elf->e_shoff) +
			elf->e_shstrndx;
	} else {
		strtab_shdr = elf_section_find(elf, ".strtab");
		if (!strtab_shdr)
			return (NULL);
	}
	if (idx >= strtab_shdr->sh_size)
		return (NULL);
	return ((void *)elf + strtab_shdr->sh_offset + idx);
}
