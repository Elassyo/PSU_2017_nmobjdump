/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF string table handling
*/

#include "nmobjdump.h"

const char *elf_strtab_get(elf_t *elf, bool section_name, uint32_t idx)
{
	Elf64_Shdr *strtab_shdr;

	if (section_name) {
		if (elf->buf->e_shstrndx == SHN_UNDEF ||
			elf->buf->e_shstrndx >= elf->buf->e_shnum)
			return (NULL);
		strtab_shdr = (Elf64_Shdr *)((void *)elf->buf +
			elf->buf->e_shoff) + elf->buf->e_shstrndx;
	} else {
		strtab_shdr = elf_section_find(elf, ".strtab");
		if (!strtab_shdr)
			return (NULL);
	}
	if (idx >= strtab_shdr->sh_size)
		return (NULL);
	return ((void *)elf->buf + strtab_shdr->sh_offset + idx);
}
