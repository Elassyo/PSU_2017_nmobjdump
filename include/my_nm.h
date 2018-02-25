/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** my_nm header
*/

#if !defined(MY_NM_H)
	#define MY_NM_H

	#include "my_fs.h"
	#include "my_ar.h"
	#include "my_elf.h"

typedef struct nm_symbol_type_map {
	char *section;
	char type;
} nm_symbol_type_map_t;

char my_nm_symbol_type(elf_t const *elf, elf_symbol_t const *sym);

#endif /* !defined(MY_NM_H) */
