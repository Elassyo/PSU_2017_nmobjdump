/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** Archive file GNU filename extensions handling
*/

#include <stdlib.h>
#include <string.h>
#include "nmobjdump.h"

const char *ar_ext_get_name_extensions_file(file_t *file)
{
	size_t off = sizeof(ar_ahdr_t);
	const ar_fhdr_t *fhdr;

	while (off < file->f_size) {
		fhdr = file->f_data + off;
		if (strncmp(fhdr->fh_ident, "//              ", 16) == 0)
			return ((const char *)(fhdr + 1));
		off += MY_ALIGN(sizeof(*fhdr) + ar_atoi(fhdr->fh_size, 10), 2);
	}
	return (NULL);
}

const char *ar_ext_get_name(file_t *file, const char *identifier)
{
	char *res;
	const char *ext;
	const char *end;

	if (identifier[0] == '/') {
		ext = ar_ext_get_name_extensions_file(file);
		if (!ext)
			return (NULL);
		identifier = ext + ar_atoi(identifier + 1, 15);
	}
	end = strchr(identifier, '/');
	res = malloc(end - identifier + 1);
	if (res) {
		strncpy(res, identifier, end - identifier);
		res[end - identifier] = 0;
	}
	return (res);
}
