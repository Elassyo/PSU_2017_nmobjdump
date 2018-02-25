/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** File system helpers
*/

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "my_fs.h"

file_t *fs_open(char const *filepath)
{
	struct stat s;
	file_t *res = malloc(sizeof(*res));

	assert(res != NULL);
	res->f_path = strdup(filepath);
	res->f_fd = open(filepath, O_RDONLY);
	if (res->f_fd == -1 || fstat(res->f_fd, &s))
		return (NULL);
	if (!S_ISREG(s.st_mode)) {
		errno = EISDIR;
		return (NULL);
	}
	res->f_size = (size_t)s.st_size;
	res->f_data = mmap(NULL, (size_t)s.st_size, PROT_READ, MAP_PRIVATE,
		res->f_fd, 0);
	return (res);
}

void fs_close(file_t *file)
{
	if (file) {
		munmap((void *)file->f_data, file->f_size);
		close(file->f_fd);
		free((char *)file->f_path);
	}
	free(file);
}
