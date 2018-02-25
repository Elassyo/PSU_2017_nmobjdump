/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** my_objdump -f mode implementation
*/

#include <stdio.h>
#include <string.h>
#include "my_objdump.h"

static objdump_elf_flags_t const OBJDUMP_ELF_FLAGS[] = {
	{ HAS_RELOC, "HAS_RELOC" },
	{ EXEC_P, "EXEC_P" },
	{ HAS_LINENO, "HAS_LINENO" },
	{ HAS_DEBUG, "HAS_DEBUG" },
	{ HAS_SYMS, "HAS_SYMS" },
	{ HAS_LOCALS, "HAS_LOCALS" },
	{ DYNAMIC, "DYNAMIC" },
	{ WP_TEXT, "WP_TEXT" },
	{ D_PAGED, "D_PAGED" },
	{ 0, NULL }
};

static unsigned int my_objdump_elf_flags(elf_t const * elf)
{
	unsigned int res = 0;
	elf_section_t const *sect;

	res |= elf->e_type == ET_EXEC ? EXEC_P : 0;
	res |= elf->e_type == ET_DYN ? DYNAMIC : 0;
	res |= elf->e_pagenum > 0 ? D_PAGED : 0;
	for (size_t i = 0; i < elf->e_sectnum; i++) {
		sect = &elf->e_sections[i];
		if ((sect->s_type == SHT_REL || sect->s_type == SHT_RELA) &&
			strcmp(sect->s_name, ".rela.dyn") != 0 &&
			strcmp(sect->s_name, ".rela.plt") != 0)
			res |= HAS_RELOC;
		res |= sect->s_type == SHT_SYMTAB ||
			sect->s_type == SHT_DYNSYM ? HAS_SYMS : 0;
		if (sect->s_size != 0 && sect->s_type != SHT_NOBITS &&
			(sect->s_flags & SHF_ALLOC) != 0 &&
			((sect->s_addr - sect->s_offset) % 0x1000) != 0)
			res &= ~D_PAGED;
	}
	return (res);
}

void my_objdump_file_header(elf_t const *elf)
{
	char *sep = "";
	unsigned int flags = my_objdump_elf_flags(elf);

	printf("architecture: %s, ", elf_info_architecture(elf));
	printf("flags 0x%08x:\n", flags);
	for (int i = 0; OBJDUMP_ELF_FLAGS[i].mask != 0; i++) {
		if (flags & OBJDUMP_ELF_FLAGS[i].mask) {
			printf("%s%s", sep, OBJDUMP_ELF_FLAGS[i].str);
			sep = ", ";
		}
	}
	putchar('\n');
	printf("start address 0x%0*lx\n", elf->e_addrsz / 4, elf->e_entry);
}
