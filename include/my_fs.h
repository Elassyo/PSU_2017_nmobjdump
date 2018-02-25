/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** Filesystem header
*/

#if !defined (MY_FS_H_)
	#define MY_FS_H_

	#include <stddef.h>

typedef struct file {
	char const *f_path;
	int f_fd;
	size_t f_size;
	void const *f_data;
} file_t;

file_t *fs_open(char const *filepath);
void fs_close(file_t *elf);

#endif /* !defined (MY_FS_H_) */
