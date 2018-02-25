/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF files information
*/

#include "my_elf.h"

char const *elf_info_format(elf_t const *elf)
{
	switch (elf->e_class) {
	case ELFCLASS32:
		return ("elf32");
	case ELFCLASS64:
		return ("elf64");
	default:
		return ("invalid");
	}
}

char const *elf_info_machine(elf_t const *elf)
{
	switch (elf->e_machine) {
	case EM_386:
		return ("i386");
	case EM_X86_64:
		return ("x86-64");
	default:
		return (elf->e_byteorder == ELFDATA2LSB ?
			"little" : "big");
	}
}

char const *elf_info_architecture(elf_t const *elf)
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
