/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF files handling
*/

#include <elf.h>
#include "nmobjdump.h"

bool elf_file_check(file_t const *file)
{
	bool ok = true;
	Elf64_Ehdr const *ehdr = file->f_data;

	if (file->f_size < sizeof(Elf64_Ehdr))
		return (0);
	ok = ok && ehdr->e_ident[EI_MAG0] == ELFMAG0;
	ok = ok && ehdr->e_ident[EI_MAG1] == ELFMAG1;
	ok = ok && ehdr->e_ident[EI_MAG2] == ELFMAG2;
	ok = ok && ehdr->e_ident[EI_MAG3] == ELFMAG3;
	ok = ok && (ehdr->e_ident[EI_CLASS] == ELFCLASS32 ||
		ehdr->e_ident[EI_CLASS] == ELFCLASS64);
	ok = ok && (ehdr->e_ident[EI_DATA] == ELFDATA2LSB ||
		ehdr->e_ident[EI_DATA] == ELFDATA2MSB);
	return (ok);
}

char const *elf_file_format(Elf64_Ehdr const *elf)
{
	switch (elf->e_ident[EI_CLASS]) {
	case ELFCLASS32:
		return ("elf32");
	case ELFCLASS64:
		return ("elf64");
	default:
		return ("invalid");
	}
}

char const *elf_file_target(Elf64_Ehdr const *elf)
{
	switch (elf->e_machine) {
	case EM_386:
		return ("i386");
	case EM_X86_64:
		return ("x86-64");
	default:
		return (elf->e_ident[EI_DATA] == ELFDATA2LSB ?
			"little" : "big");
	}
}

char const *elf_file_architecture(Elf64_Ehdr const *elf)
{
	switch (elf->e_machine) {
	case EM_386:
		return ("i386");
	case EM_X86_64:
		return ("i386:x86-64");
	default:
		return ("UNKNOWN!");
	}
}
