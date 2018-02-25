/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** Filesystem header
*/

#if !defined (MY_FS_H_)
	#define MY_FS_H_

	#include <errno.h>
	#include <stdbool.h>
	#include <stddef.h>

typedef struct file {
	char const *f_path;
	int f_fd;
	size_t f_size;
	void const *f_data;
} file_t;

file_t *fs_open(char const *path);
void fs_close(file_t *elf);

inline __attribute__ ((always_inline)) bool fs_check(
	file_t const *file, size_t offset, size_t size)
{
	bool res = offset <= file->f_size && size <= file->f_size - offset;

	if (!res) {
		errno = EIO;
	}
	return (res);
}

#endif /* !defined (MY_FS_H_) */
