/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** my_objdump -f mode implementation
*/

#include <stdio.h>
#include "my_objdump.h"

void my_objdump_file_header(elf_t const *elf)
{
	printf("architecture: %s, ", elf_info_architecture(elf));
	printf("flags 0x%08x:\n\n", 0);
	printf("start address 0x%0*lx\n\n", elf->e_addrsz / 4, elf->e_entry);
}
