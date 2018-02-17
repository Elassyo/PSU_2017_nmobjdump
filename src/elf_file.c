/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF files handling
*/

#include <elf.h>
#include "nmobjdump.h"

bool elf_file_check(file_t *file)
{
	bool ok = true;
	const Elf64_Ehdr *ehdr = file->f_data;

	if (file->f_size < sizeof(Elf64_Ehdr))
		return (0);
	ok = ok && ehdr->e_ident[EI_MAG0] == ELFMAG0;
	ok = ok && ehdr->e_ident[EI_MAG1] == ELFMAG1;
	ok = ok && ehdr->e_ident[EI_MAG2] == ELFMAG2;
	ok = ok && ehdr->e_ident[EI_MAG3] == ELFMAG3;
	ok = ok && ehdr->e_ident[EI_CLASS] == ELFCLASS64;
	ok = ok && ehdr->e_ident[EI_DATA] == ELFDATA2LSB;
	return (ok);
}
