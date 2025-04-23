#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * read_line - reads one line from stdin via getline.
 * Return: malloc’d line (caller must free), or NULL on EOF/error.
 */
char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;

	if (getline(&line, &len, stdin) == -1)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * trim_newline - replaces trailing '\n' with '\0'.
 * @s: line to modify.
 */
void trim_newline(char *s)
{
	size_t i = 0;

	if (!s)
		return;
	while (s[i] && s[i] != '\n')
		i++;
	s[i] = '\0';
}

/**
 * is_blank - checks if a string is all spaces/tabs or empty.
 * @s: string to check.
 * Return: 1 if blank or NULL, 0 otherwise.
 */
int is_blank(const char *s)
{
	if (!s)
		return (1);
	while (*s)
	{
		if (*s != ' ' && *s != '\t')
			return (0);
		s++;
	}
	return (1);
}

