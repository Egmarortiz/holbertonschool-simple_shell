#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * read_line - reads one line from stdin via getline
 * Return: malloc’d line, or NULL on EOF/error
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
 * trim_newline - replaces trailing '\n' with '\0'
 * @s: line to modifiy
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
 * is_blank - checks if a string is all spaces/tabs or empty
 * @s: string to check
 * Return: 1 if blank, 0 otherwise
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

/**
 * split_line - splits a line into whitespace-delimited tokens
 * @line: input string (modified in place)
 * Return: NULL-terminated array of pointers, or NULL on alloc error
 */
char **split_line(char *line)
{
	char **args;
	size_t bufsize = 64, pos = 0;
	char *token;

	args = malloc(bufsize * sizeof(char *));
	if (!args)
		return (NULL);

	token = strtok(line, " \t");
	while (token)
	{
		args[pos++] = token;
		if (pos >= bufsize)
		{
			bufsize *= 2;
			args = realloc(args, bufsize * sizeof(char *));
			if (!args)
				return (NULL);
		}
		token = strtok(NULL, " \t");
	}
	args[pos] = NULL;
	return (args);
}

/**
 * free_args - frees the argument array
 * @args: array returned by split_line
 */
void free_args(char **args)
{
	free(args);
}

