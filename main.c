#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * handle_builtin - Handle built-in shell commands
 * @args: Array of command arguments
 * @line: Original input line (for freeing)
 *
 * This function handles the built-in shell commands:
 * - "exit": exits the shell
 * - "env": prints the environment variables
 *
 * Return: 1 if builtin was handled, 0 otherwise
 */
int handle_builtin(char **args, char *line)
{
	size_t i;

	if (strcmp(args[0], "exit") == 0)
	{
		free_args(args);
		free(line);
		exit(0);
	}
	if (strcmp(args[0], "env") == 0)
	{
		for (i = 0; environ[i]; i++)
			printf("%s\n", environ[i]);
		free_args(args);
		free(line);
		return (1);
	}
	return (0);
}

/**
 * main - entry point for simple_shell with built-ins exit and env
 * @argc: arg count (unused)
 * @argv: arg vector; argv[0] used for errors
 * Return: 0 on exit
 */
int main(int argc, char **argv)
{
	char *line;
	char **args;

	(void)argc;
	while (1)
	{
		print_prompt();
		line = read_line();
		if (!line)
			break;  /* EOF or error */
		trim_newline(line);
		if (is_blank(line))
		{
			free(line);
			continue;
		}
		args = split_line(line);
		if (args[0] == NULL)  /* no tokens */
		{
			free_args(args);
			free(line);
			continue;
		}
		if (handle_builtin(args, line))
			continue;
		execute_command(args, argv[0]);
		free_args(args);
		free(line);
	}
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
