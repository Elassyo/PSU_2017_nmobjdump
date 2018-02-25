/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF files handling
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "my_elf.h"

bool elf_file_check(file_t const *file)
{
	bool ok = true;
	Elf32_Ehdr const *ehdr = file->f_data;

	if (!elf_offset_check(file, 0, sizeof(Elf32_Ehdr)))
		return (0);
	ok = memcmp(ehdr->e_ident, ELFMAG, SELFMAG) == 0;
	ok = ok && (ehdr->e_ident[EI_CLASS] == ELFCLASS32 ||
		ehdr->e_ident[EI_CLASS] == ELFCLASS64);
	ok = ok && (ehdr->e_ident[EI_DATA] == ELFDATA2LSB ||
		ehdr->e_ident[EI_DATA] == ELFDATA2MSB);
	ok = ok && ehdr->e_ident[EI_VERSION] == EV_CURRENT;
	return (ok);
}

elf_t *elf_file_open(file_t const *file)
{
	Elf32_Ehdr const *ehdr = file->f_data;
	elf_t *elf = malloc(sizeof(*elf));

	assert(elf != NULL);
	elf->e_class = ehdr->e_ident[EI_CLASS];
	elf->e_byteorder = ehdr->e_ident[EI_DATA];
	elf->e_machine = ehdr->e_machine;
	if (elf->e_class == ELFCLASS32) {
		elf->e_addrsz = sizeof(Elf32_Addr) * 8;
		if (elf->e_byteorder == ELFDATA2LSB)
			return (elf_file_open_32le(file, elf));
		else
			return (elf_file_open_32be(file, elf));
	} else {
		elf->e_addrsz = sizeof(Elf64_Addr) * 8;
		if (elf->e_byteorder == ELFDATA2LSB)
			return (elf_file_open_64le(file, elf));
		else
			return (elf_file_open_64be(file, elf));
	}
}

void elf_file_close(elf_t *elf)
{
	if (elf) {
		free((void *)elf->e_sections);
		free((void *)elf->e_symbols);
	}
	free(elf);
}
