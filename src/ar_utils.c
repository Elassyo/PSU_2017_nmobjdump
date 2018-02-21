/*
** EPITECH PROJECT, 2018
** PSU_2017_nmobjdump
** File description:
** $END
*/

#include "nmobjdump.h"

int ar_atoi(char const *str, int len)
{
	int res = 0;

	for (int i = 0; i < len && str[i] != ' '; i++)
		res = res * 10 + str[i] - '0';
	return (res);
}
