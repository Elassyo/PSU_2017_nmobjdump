/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** File handling routines
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "nmobjdump.h"

int elf_open(const char *progname, const char *filepath, elf_t *elf)
{
	struct stat s;

	elf->fd = open(filepath, O_RDONLY);
	if (elf->fd == -1 || fstat(elf->fd, &s)) {
		fprintf(stderr, "%s: '%s': %m\n", progname, filepath);
		return (84);
	}
	if (!S_ISREG(s.st_mode)) {
		fprintf(stderr, "%s: '%s': Not a file\n", progname, filepath);
		return (84);
	}
	elf->size = (size_t)s.st_size;
	elf->buf = mmap(NULL, (size_t)s.st_size, PROT_READ, MAP_PRIVATE,
		elf->fd, 0);
	if (elf->buf == (void *)-1) {
		fprintf(stderr, "%s: mmap failed: %m\n", progname);
		return (84);
	}
	return (0);
}

void elf_close(elf_t *elf)
{
	munmap(elf->buf, elf->size);
	close(elf->fd);
}
