/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF files handling (ELF64 little endian)
*/

#include <assert.h>
#include <stdlib.h>
#include <elf.h>
#include "my_elf.h"

static char const *elf_section_name(file_t const *file, unsigned int idx)
{
	Elf64_Ehdr const *ehdr = file->f_data;
	Elf64_Shdr const *shdr;
	char const *strtab;

	if (ehdr->e_shstrndx == SHN_UNDEF || ehdr->e_shstrndx >= ehdr->e_shnum)
		return (NULL);
	shdr = &((Elf64_Shdr const *)(file->f_data + ehdr->e_shoff))
		[ehdr->e_shstrndx];
	if (!elf_offset_check(file, shdr->sh_offset, shdr->sh_size))
		return (NULL);
	strtab = file->f_data + shdr->sh_offset;
	return (idx < shdr->sh_size ? strtab + idx : NULL);
}

static elf_section_t *elf_section_read(file_t const *file, size_t *sectnum_ptr)
{
	Elf64_Ehdr const *ehdr = file->f_data;
	Elf64_Shdr const *shtab = file->f_data + ehdr->e_shoff;
	elf_section_t *res = malloc(ehdr->e_shnum * sizeof(*res));

	assert(res != NULL);
	if (ehdr->e_shentsize != sizeof(*shtab) || !elf_offset_check(file,
		ehdr->e_shoff, ehdr->e_shnum * sizeof(*shtab)))
		return (NULL);
	for (size_t i = 0; i < ehdr->e_shnum; i++) {
		res[i].s_name = elf_section_name(file, shtab[i].sh_name);
		res[i].s_type = shtab[i].sh_type;
		res[i].s_flags = shtab[i].sh_flags;
		res[i].s_size = shtab[i].sh_size;
		res[i].s_entsize = shtab[i].sh_entsize;
		res[i].s_data = file->f_data + shtab[i].sh_offset;
		if ((res[i].s_type != SHT_NOBITS && !elf_offset_check(file,
			shtab[i].sh_offset, res[i].s_size)) || !res[i].s_name)
			return (NULL);
	}
	*sectnum_ptr = ehdr->e_shnum;
	return (res);
}

static elf_symbol_t *elf_symbol_read(elf_section_t const *symtab_s,
	elf_section_t const *strtab_s, size_t *symnum_ptr)
{
	elf_symbol_t *res;
	Elf64_Sym const *symtab;

	if (symtab_s->s_entsize != sizeof(*symtab))
		return (NULL);
	symtab = symtab_s->s_data;
	res = malloc(symtab_s->s_size / sizeof(*symtab) * sizeof(*res));
	assert(res != NULL);
	for (size_t i = 0; i < symtab_s->s_size / sizeof(*symtab); i++) {
		if (symtab[i].st_name > strtab_s->s_size)
			return (NULL);
		res[i].sym_name = strtab_s->s_data + symtab[i].st_name;
		res[i].sym_value = symtab[i].st_value;
		res[i].sym_type = ELF64_ST_TYPE(symtab[i].st_info);
		res[i].sym_bind = ELF64_ST_BIND(symtab[i].st_info);
		res[i].sym_sectidx = symtab[i].st_shndx;
	}
	*symnum_ptr = symtab_s->s_size / sizeof(*symtab);
	return (res);
}

elf_t *elf_file_open_64le(file_t const *file, elf_t *elf)
{
	elf_section_t const *symtab_s;
	elf_section_t const *strtab_s;

	if (!elf_offset_check(file, 0, sizeof(Elf64_Ehdr)))
		return (NULL);
	elf->e_entry = ((Elf64_Ehdr const *)file->f_data)->e_entry;
	elf->e_sectnum = 0;
	elf->e_sections = elf_section_read(file, &elf->e_sectnum);
	if (!elf->e_sections)
		return (NULL);
	symtab_s = elf_section_find(elf, ".symtab", SHT_SYMTAB);
	strtab_s = elf_section_find(elf, ".strtab", SHT_STRTAB);
	elf->e_symnum = 0;
	if (symtab_s && strtab_s)
		elf->e_symbols = elf_symbol_read(symtab_s, strtab_s,
			&elf->e_symnum);
	else
		elf->e_symbols = NULL;
	return (elf);
}
