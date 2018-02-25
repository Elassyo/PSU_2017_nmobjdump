/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** my_objdump -s mode implementation
*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "my_objdump.h"

static bool is_ignored_section(elf_section_t const *sect)
{
	if (strncmp(sect->s_name, ".rel", 4) == 0)
		return (strcmp(sect->s_name, ".rela.dyn") != 0 &&
			strcmp(sect->s_name, ".rela.plt") != 0);
	return (sect->s_type == SHT_NOBITS ||
		strcmp(sect->s_name, ".symtab") == 0 ||
		strcmp(sect->s_name, ".strtab") == 0 ||
		strcmp(sect->s_name, ".shstrtab") == 0);
}

static int count_hex_digits(unsigned long nbr)
{
	int res = 1;

	while (nbr >= 16) {
		nbr /= 16;
		res++;
	}
	return (res);
}

static void my_objdump_full_content_line(unsigned char const *data, int len,
	unsigned long addr, int addr_len)
{
	printf(" %0*lx", addr_len, addr);
	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0)
			putchar(' ');
		if (i < len)
			printf("%02hhx", data[i]);
		else
			printf("  ");
	}
	printf("  ");
	for (int i = 0; i < 16; i++) {
		if (i < len)
			putchar(isprint(data[i]) ? data[i] : '.');
		else
			putchar(' ');
	}
	putchar('\n');
}

void my_objdump_full_content(elf_t const *elf)
{
	int addr_len;
	elf_section_t const *sect;

	puts("");
	for (unsigned long i = 1; i < elf->e_sectnum; i++) {
		sect = &elf->e_sections[i];
		if (sect->s_size == 0 || is_ignored_section(sect))
			continue;
		printf("Contents of section %s:\n", sect->s_name);
		addr_len = MY_MAX(count_hex_digits(sect->s_addr +
			sect->s_size + 1), 4);
		for (unsigned long off = 0; off < sect->s_size; off += 16) {
			my_objdump_full_content_line(sect->s_data + off,
				sect->s_size - off > 16 ?
					16 : sect->s_size - off,
				sect->s_addr + off, addr_len);
		}
	}
}
