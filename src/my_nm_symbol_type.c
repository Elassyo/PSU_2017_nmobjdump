/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** nm symbol type handling
*/

#include <ctype.h>
#include <string.h>
#include "my_nm.h"

static nm_symbol_type_map_t const SYMBOL_TYPE_MAP[] = {
	{ ".bss", 'b' },
	{ ".data", 'd' },
	{ ".debug", 'n' },
	{ ".dynamic", 'd' },
	{ ".eh_frame", 'r' },
	{ ".fini", 't' },
	{ ".got", 'd' },
	{ ".init", 't' },
	{ ".rdata", 'r' },
	{ ".rodata", 'r' },
	{ ".sbss", 's' },
	{ ".scommon", 'c' },
	{ ".sdata", 'g' },
	{ ".tbss", 'b' },
	{ ".tdata", 'd' },
	{ ".text", 't' },
	{ 0, 0 }
};

static char my_nm_symbol_section_to_type(elf_t const *elf, size_t sectidx)
{
	elf_section_t const *sect;

	if (sectidx >= elf->e_sectnum)
		return ('?');
	sect = &elf->e_sections[sectidx];
	for (nm_symbol_type_map_t const *m = SYMBOL_TYPE_MAP; m->section; m++) {
		if (strncmp(m->section, sect->s_name, strlen(m->section)) == 0)
			return (m->type);
	}
	if (sect->s_type == SHT_NOBITS)
		return ('b');
	if (sect->s_type == SHT_PROGBITS && sect->s_flags & SHF_EXECINSTR)
		return ('t');
	return (sect->s_flags & SHF_WRITE ? 'd' : 'r');
}

char my_nm_symbol_type(elf_t const *elf, elf_symbol_t const *sym)
{
	char c;

	if (sym->sym_name[0] == 0 || sym->sym_type == STT_FILE)
		return (0);
	if (sym->sym_type == STT_GNU_IFUNC)
		return ('i');
	if (sym->sym_bind == STB_WEAK) {
		c = sym->sym_type == STT_OBJECT ? 'v' : 'w';
		return (sym->sym_sectidx == SHN_UNDEF ? c : toupper(c));
	}
	if (sym->sym_sectidx == SHN_ABS)
		return ('A');
	if (sym->sym_sectidx == SHN_COMMON)
		return ('C');
	if (sym->sym_sectidx == SHN_UNDEF)
		return ('U');
	c = my_nm_symbol_section_to_type(elf, sym->sym_sectidx);
	return (sym->sym_bind == STB_GLOBAL ? toupper(c) : c);
}
