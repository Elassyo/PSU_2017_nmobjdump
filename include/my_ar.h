/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** archive file header
*/

#if !defined (MY_AR_H_)
	#define MY_AR_H_

	#include <stdbool.h>
	#include "my_fs.h"

	#define MY_ALIGN(x, align) (((x) + (align) - 1) & ~((align) - 1))

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

#endif /* !defined (MY_AR_H_) */
