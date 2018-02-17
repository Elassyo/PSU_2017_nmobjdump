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

	#define MY_ALIGN(x, align) (((x) + (align) - 1) & ~((align) - 1))

/* FS */

typedef struct file {
	const char *f_path;
	int f_fd;
	size_t f_size;
	const void *f_data;
} file_t;

file_t *fs_open(const char *progname, const char *filepath);
void fs_close(file_t *elf);

/* AR */

	#define AR_SIG "!<arch>\n"
	#define AR_SIG_SIZE 8

typedef struct ar_ahdr {
	char a_sig[AR_SIG_SIZE];
} __attribute__ ((packed)) ar_ahdr_t;

	#define AR_END "`\n"
	#define AR_END_SIZE 2

typedef struct ar_fhdr {
	char fh_ident[16];
	char fh_mtime[12];
	char fh_uid[6];
	char fh_gid[6];
	char fh_mode[8];
	char fh_size[10];
	char fh_end[AR_END_SIZE];
} __attribute__ ((packed)) ar_fhdr_t;

typedef struct ar_file {
	file_t af_file;
	size_t af_off;
} ar_file_t;

int ar_atoi(const char *str, int len);
bool ar_file_check(file_t *file);
ar_file_t *ar_file_iterate(file_t *file, ar_file_t *prev);
const char *ar_ext_get_name(file_t *file, const char *identifier);

/* ELF */

typedef struct elf_symbol {
	const char *name;
	const Elf64_Sym *sym;
} elf_symbols_t;

bool elf_file_check(file_t *file);

const Elf64_Shdr *elf_section_find(const Elf64_Ehdr *elf, const char *name);
const Elf64_Shdr *elf_section_get(const Elf64_Ehdr *elf, Elf64_Section idx);

const char *elf_strtab_get(const Elf64_Ehdr *elf, bool section_name,
	uint32_t idx);

elf_symbols_t *elf_symbols_read_symtab(const Elf64_Ehdr *elf,
	const Elf64_Sym *symtab, uint64_t len);
char elf_symbols_get_type(const Elf64_Ehdr *elf, const Elf64_Sym *sym);
int elf_symbols_sorter(const void *a, const void *b);

/* NM */

typedef struct nm_sect2type_map {
	char *section;
	char type;
} nm_sect2type_map_t;

/* OBJDUMP */

#endif /* !defined (NMOBJDUMP_H_) */
