/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** Archive file miscellaneous functions
*/

#include "my_ar.h"

int ar_atoi(char const *str, int len)
{
	int res = 0;

	for (int i = 0; i < len && str[i] != ' '; i++)
		res = res * 10 + str[i] - '0';
	return (res);
}
