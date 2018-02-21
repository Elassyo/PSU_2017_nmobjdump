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
	char const *f_path;
	int f_fd;
	size_t f_size;
	void const *f_data;
} file_t;

file_t *fs_open(char const *progname, char const *filepath);
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

int ar_atoi(char const *str, int len);
bool ar_file_check(file_t const *file);
ar_file_t *ar_file_iterate(file_t const *file, ar_file_t *prev);
char const *ar_ext_get_name(file_t const *file, char const *identifier);

/* ELF */

typedef struct elf_symbol {
	char const *name;
	Elf64_Sym const *sym;
} elf_symbols_t;

bool elf_file_check(file_t const *file);
char const *elf_file_format(Elf64_Ehdr const *elf);
char const *elf_file_target(Elf64_Ehdr const *elf);
char const *elf_file_architecture(Elf64_Ehdr const *elf);

Elf64_Shdr const *elf_section_find(Elf64_Ehdr const *elf, char const *name);
Elf64_Shdr const *elf_section_get(Elf64_Ehdr const *elf, Elf64_Section idx);

char const *elf_strtab_get(Elf64_Ehdr const *elf, bool section_name,
	uint32_t idx);

elf_symbols_t *elf_symbols_read_symtab(Elf64_Ehdr const *elf,
	Elf64_Sym const *symtab, uint64_t len);
char elf_symbols_get_type(Elf64_Ehdr const *elf, Elf64_Sym const *sym);
int elf_symbols_sorter(void const *a, void const *b);

/* NM */

typedef struct nm_sect2type_map {
	char *section;
	char type;
} nm_sect2type_map_t;

/* OBJDUMP */

typedef struct objdump_options {
	bool file_header;
	bool full_content;
} objdump_options_t;

void my_objdump_file_header(Elf64_Ehdr const *elf);
void my_objdump_full_content(Elf64_Ehdr const *elf);

#endif /* !defined (NMOBJDUMP_H_) */
