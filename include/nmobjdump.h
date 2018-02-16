/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** nm and objdump file header
*/

#if !defined (NMOBJDUMP_H_)
	#define NMOBJDUMP_H_

	#include <elf.h>
	#include <stdbool.h>
	#include <stddef.h>

/* ELF */

typedef struct elf {
	int fd;
	size_t size;
	Elf64_Ehdr *buf;
} elf_t;

typedef struct elf_sym {
	const char *name;
	Elf64_Sym *sym;
} elf_sym_t;

int elf_open(const char *progname, const char *filepath, elf_t *elf);
void elf_close(elf_t *elf);

Elf64_Shdr *elf_section_find(elf_t *elf, const char *name);
Elf64_Shdr *elf_section_get(elf_t *elf, Elf64_Section idx);

const char *elf_strtab_get(elf_t *elf, bool section_name, uint32_t idx);

elf_sym_t *elf_symtab(elf_t *elf, Elf64_Sym *symtab_data, uint64_t length);
int elf_symtab_sort(const void *a, const void *b);

/* NM */

typedef struct nm_sect2type_map {
	char *section;
	char type;
} nm_sect2type_map_t;

/* OBJDUMP */

#endif /* !defined (NMOBJDUMP_H_) */
