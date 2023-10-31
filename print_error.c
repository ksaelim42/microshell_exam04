#include "microshell.h"

void err(char *s)
{
	while (*s)
		write(2, s++, 1);
}

int errl(char *s)
{
	err(s);
	err("\n");
	return (1);
}

int errl_par(char *s1, char *s2)
{
	err(s1);
	err(" ");
	errl(s2);
	return (1);
}