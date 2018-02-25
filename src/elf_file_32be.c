/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF files handling (ELF32 big endian)
*/

#include <assert.h>
#include <stdlib.h>
#include <elf.h>
#include "my_elf.h"

static char const *elf_section_name(file_t const *file, unsigned int idx)
{
	Elf32_Ehdr const *e = file->f_data;
	Elf32_Shdr const *s;
	char const *t;

	if (BS16(e->e_shstrndx) == SHN_UNDEF ||
		BS16(e->e_shstrndx) >= BS16(e->e_shnum))
		return (NULL);
	s = &((Elf32_Shdr const *)(file->f_data + BS32(e->e_shoff)))
	[BS16(e->e_shstrndx)];
	if (!elf_offset_check(file, BS32(s->sh_offset), BS32(s->sh_size)))
		return (NULL);
	t = file->f_data + BS32(s->sh_offset);
	return (idx < BS32(s->sh_size) ? t + idx : NULL);
}

static elf_section_t *elf_section_read(file_t const *f, size_t *sectnum_ptr)
{
	Elf32_Ehdr const *e = f->f_data;
	Elf32_Shdr const *s = f->f_data + BS32(e->e_shoff);
	elf_section_t *res = malloc(BS16(e->e_shnum) * sizeof(*res));

	assert(res != NULL);
	if (BS16(e->e_shentsize) != sizeof(*s) || !elf_offset_check(f,
		BS32(e->e_shoff), BS16(e->e_shnum) * sizeof(*s)))
		return (NULL);
	for (size_t i = 0; i < BS16(e->e_shnum); i++) {
		res[i].s_name = elf_section_name(f, BS32(s[i].sh_name));
		res[i].s_type = BS32(s[i].sh_type);
		res[i].s_flags = BS32(s[i].sh_flags);
		res[i].s_size = BS32(s[i].sh_size);
		res[i].s_entsize = BS32(s[i].sh_entsize);
		res[i].s_data = f->f_data + BS32(s[i].sh_offset);
		if ((res[i].s_type != SHT_NOBITS && !elf_offset_check(f,
			BS32(s[i].sh_offset), res[i].s_size)) || !res[i].s_name)
			return (NULL);
	}
	*sectnum_ptr = BS16(e->e_shnum);
	return (res);
}

static elf_symbol_t *elf_symbol_read(elf_section_t const *symtab_s,
	elf_section_t const *strtab_s, size_t *symnum_ptr)
{
	elf_symbol_t *res;
	Elf32_Sym const *symtab;

	if (symtab_s->s_entsize != sizeof(*symtab))
		return (NULL);
	symtab = symtab_s->s_data;
	res = malloc(symtab_s->s_size / sizeof(*symtab) * sizeof(*res));
	assert(res != NULL);
	for (size_t i = 0; i < symtab_s->s_size / sizeof(*symtab); i++) {
		if (BS32(symtab[i].st_name) > strtab_s->s_size)
			return (NULL);
		res[i].sym_name = strtab_s->s_data + BS32(symtab[i].st_name);
		res[i].sym_value = BS32(symtab[i].st_value);
		res[i].sym_type = ELF32_ST_TYPE(symtab[i].st_info);
		res[i].sym_bind = ELF32_ST_BIND(symtab[i].st_info);
		res[i].sym_sectidx = BS16(symtab[i].st_shndx);
	}
	*symnum_ptr = symtab_s->s_size / sizeof(*symtab);
	return (res);
}

elf_t *elf_file_open_32be(file_t const *file, elf_t *elf)
{
	elf_section_t const *symtab_s;
	elf_section_t const *strtab_s;

	if (!elf_offset_check(file, 0, sizeof(Elf32_Ehdr)))
		return (NULL);
	elf->e_entry = BS32(((Elf32_Ehdr const *)file->f_data)->e_entry);
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
