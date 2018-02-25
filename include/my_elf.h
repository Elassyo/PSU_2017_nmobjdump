/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** elf file header
*/

#if !defined (MY_ELF_H_)
	#define MY_ELF_H_

	#include <elf.h>
	#include <stdbool.h>
	#include <stddef.h>
	#include "my_fs.h"

	#define BS16 __builtin_bswap16
	#define BS32 __builtin_bswap32
	#define BS64 __builtin_bswap64

typedef struct elf_section {
	char const *s_name;
	unsigned int s_type;
	unsigned long s_flags;
	size_t s_size;
	size_t s_entsize;
	unsigned long s_addr;
	unsigned long s_offset;
	void const *s_data;
} elf_section_t;

typedef struct elf_symbol {
	char const *sym_name;
	uintptr_t sym_value;
	unsigned char sym_type;
	unsigned char sym_bind;
	unsigned short sym_sectidx;
} elf_symbol_t;

typedef struct elf {
	unsigned char e_class;
	unsigned char e_byteorder;
	unsigned short e_machine;
	unsigned short e_type;
	int e_addrsz;
	unsigned long e_entry;
	elf_section_t const *e_sections;
	size_t e_sectnum;
	size_t e_pagenum;
	elf_symbol_t const *e_symbols;
	size_t e_symnum;
} elf_t;

bool elf_file_check(file_t const *file);
elf_t *elf_file_open(file_t const *file);
elf_t *elf_file_open_32le(file_t const *file, elf_t *elf);
elf_t *elf_file_open_32be(file_t const *file, elf_t *elf);
elf_t *elf_file_open_64le(file_t const *file, elf_t *elf);
elf_t *elf_file_open_64be(file_t const *file, elf_t *elf);
void elf_file_close(elf_t *elf);

char const *elf_info_format(elf_t const *elf);
char const *elf_info_machine(elf_t const *elf);
char const *elf_info_architecture(elf_t const *elf);

elf_section_t const *elf_section_find(elf_t const *elf,
	char const *name, unsigned int type);

int elf_symbol_cmp(void const *a, void const *b);

#endif /* !defined (MY_ELF_H_) */
