/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** ELF symbols handling
*/

#include <ctype.h>
#include "my_elf.h"

int elf_symbol_cmp(void const *a, void const *b)
{
	char const *a_name = ((elf_symbol_t *)a)->sym_name;
	char const *b_name = ((elf_symbol_t *)b)->sym_name;

	while (*a_name && *b_name &&
		(toupper(*a_name) == toupper(*b_name) ||
		!isalnum(*a_name) || !isalnum(*b_name))) {
		if (toupper(*a_name) == toupper(*b_name)) {
			a_name++;
			b_name++;
		}
		if (*a_name && !isalnum(*a_name))
			a_name++;
		if (*b_name && !isalnum(*b_name))
			b_name++;
	}
	if (toupper(*a_name) == toupper(*b_name))
		return ((int)(a_name - b_name));
	return (toupper(*a_name) - toupper(*b_name));
}
