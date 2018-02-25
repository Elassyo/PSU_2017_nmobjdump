/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** my_nm header
*/


#if !defined(MY_OBJDUMP_H)
	#define MY_OBJDUMP_H

	#include <stdbool.h>
	#include "my_fs.h"
	#include "my_ar.h"
	#include "my_elf.h"

	#define MY_MIN(a, b) ((a) < (b) ? (a) : (b))
	#define MY_MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct objdump_options {
	bool file_header;
	bool full_content;
} objdump_options_t;

void my_objdump_file_header(elf_t const *elf);
void my_objdump_full_content(elf_t const *elf);

#endif /* !defined(MY_OBJDUMP_H) */
