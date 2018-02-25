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

	#define HAS_RELOC 0x01
	#define EXEC_P 0x02
	#define HAS_LINENO 0x04
	#define HAS_DEBUG 0x08
	#define HAS_SYMS 0x10
	#define HAS_LOCALS 0x20
	#define DYNAMIC 0x40
	#define WP_TEXT 0x80
	#define D_PAGED 0x100

typedef struct objdump_options {
	bool file_header;
	bool full_content;
} objdump_options_t;

typedef struct objdump_elf_flags {
	unsigned int mask;
	char const *str;
} objdump_elf_flags_t;

void my_objdump_file_header(elf_t const *elf);
void my_objdump_full_content(elf_t const *elf);

#endif /* !defined(MY_OBJDUMP_H) */
