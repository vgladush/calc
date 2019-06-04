#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// if between numbers and signs at least one space
char **my_split(const char *s) //split with check valid
{
	// valid
	int spc = 0, brack = 0, begin = 0; 
	while (s[begin] && (s[begin] == ' ' || s[begin] == '\t'))
		++begin; for
	for (int i = begin; s[i]; ++i)
	{
		if (s[i] == ' ' || s[i] == '\t')
		{
			while (s[i] && (s[i] == ' ' || s[i] == '\t'))
				++i;
			--i;
			++spc;
		}
		else if (s[i] == '(')
		{
			spc += 2; // "(" + ")"
			++brack;
		}
		else if (s[i] == ')')
		{
			if (--brack < 0)
			{
				printf("error: can't be \")\" before \"(\"\n");
				exit(1);
			}
		}
		else if ((s[i] > 57 || s[i] < 48) && // if not a number 48 == 0 etc.
			s[i] != '+' && s[i] != '-' && s[i] != '/' && s[i] != '*')
		{
			printf("error: such char (%c) is forbidden\n", s[i]);
			exit(1);
		}
	}
	if (brack != 0)
	{
		printf("error: there is no \")\"\n");
		exit(1);
	}
	char **res = (char **)malloc(sizeof(char *) * (spc + 2)); // +2 because spaces between 2 pieces and +1 for last pointer (NULL)
	res[spc + 1] = 0; // NULL

	int part = 0;

	//split
	for (int i = begin; s[i]; ++part)
	{
		if (s[i] == ')' || s[i] == '(')
		{
			res[part] = (char *)malloc(sizeof(char) * 2);
			res[part][0] = s[i++];
			res[part][1] = 0; // '\0'
		}
		else
		{
			int cn = i;
			while (s[cn] && s[cn] != ' ' && s[cn] != '\t' && s[cn] != ')' && s[cn] != '(')
				++cn;
			res[part] = (char *)malloc(sizeof(char) * (cn - i + 1));
			memcpy(res[part], s + i, cn - i);
			res[part][cn - i] = 0;
			i = cn;
		}
		while (s[i] == ' ' || s[i] == '\t')
			++i;
	}
	return res;
}

void del_part(char **s)
{
	free(*s);
	int i = 0;
	do
	{
		s[i] = s[i + 1];
		++i;
	} while (s[i]);
}

int calc(char **s)
{
	int res, sec, first;
	char symb, oper, bl = 0;

	if (**s == '(')
		res = calc(s + 1);
	else
		res = atoi(*s);
	del_part(s);

	while (*s)
	{

		symb = **s;
		del_part(s);

		if (symb == ')')
			return res;

		if (!*s)
		{
			printf("error: not full expression\n");
			exit(1);
		}

		if (**s == '(')
			sec = calc(s + 1);
		else
			sec = atoi(*s);

		del_part(s);

		if (*s && (**s == '*' || **s == '/'))
		{
			if (bl)
				first += (oper == '+' ? res : -res);
			else
				first = res;
			bl = 1;
			oper = symb;
			res = sec;
		}
		else if (symb == '*')
			res *= sec;
		else if (symb == '/')
		{
			if (!sec)
			{
				printf("error: division by 0\n");
				exit(1);
			}
			res /= sec;
		}
		else if (symb == '+' || symb == '-')
		{
			if (bl)
			{
				bl = 0;
				res = first + (oper == '+' ? res : -res);
			}
			res += (symb == '+' ? sec : -sec);
		}
		else if (symb == '-')
			res -= sec;
		else
		{
			printf("error: no such operator \n");
			exit(1);
		}
	}
	if (bl)
		res = first + (oper == '+' ? res : -res);

	return res;
}

int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		printf("usage: ./calc \"1 + (2 - 3) * -4 / 5\"\n");
		return 1;
	}

	char **s = my_split(argv[1]);
	printf("%d\n", calc(s));
	free(s);
	return 0;
}
