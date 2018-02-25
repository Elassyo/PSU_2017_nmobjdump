/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** Archive files handling
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "my_ar.h"

static ar_file_t *ar_file_open(file_t const *file, size_t off)
{
	ar_fhdr_t const *fhdr = file->f_data + off;
	ar_file_t *res = malloc(sizeof(*res));

	assert(res != NULL);
	res->af_off = off;
	res->af_file.f_path = ar_ext_get_name(file, fhdr->fh_ident);
	res->af_file.f_size = (size_t)ar_atoi(fhdr->fh_size, 10);
	res->af_file.f_data = fhdr + 1;
	return (res);
}

static void ar_file_close(ar_file_t *ar_file)
{
	if (ar_file) {
		free((char *)ar_file->af_file.f_path);
	}
	free(ar_file);
}

bool ar_file_check(file_t const *file)
{
	const ar_ahdr_t *ar = file->f_data;

	if (file->f_size < sizeof(ar_ahdr_t))
		return (0);
	return (memcmp(ar->a_sig, AR_SIG, AR_SIG_SIZE) == 0);
}

ar_file_t *ar_file_iterate(file_t const *file, ar_file_t *prev)
{
	size_t off = MY_ALIGN(!prev ? sizeof(ar_ahdr_t) :
		prev->af_off + sizeof(ar_fhdr_t) + prev->af_file.f_size, 2);
	const ar_fhdr_t *fhdr;

	ar_file_close(prev);
	while (off < file->f_size) {
		fhdr = file->f_data + off;
		if (strncmp(fhdr->fh_ident, "/               ", 16) != 0 &&
			strncmp(fhdr->fh_ident, "//              ", 16) != 0)
			return (ar_file_open(file, off));
		off += MY_ALIGN(sizeof(*fhdr) + ar_atoi(fhdr->fh_size, 10), 2);
	}
	return (NULL);
}
