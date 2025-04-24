#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/**
 * print_environment - prints all environment variables
 * Return: void
 */
void print_environment(void)
{
	size_t i;

	for (i = 0; environ[i]; i++)
		printf("%s\n", environ[i]);
}

/**
 * handle_command - processes and executes the given command
 * @args: array of command arguments
 * @line: original input line (for freeing)
 * @prog_name: program name for error messages
 * Return: 1 to continue, 0 to exit
 */
int handle_command(char **args, char *line, const char *prog_name)
{
	if (strcmp(args[0], "exit") == 0)
	{
		free_args(args);
		free(line);
		return (0); /* exit the shell */
	}
	if (strcmp(args[0], "env") == 0)
	{
		print_environment();
		free_args(args);
		free(line);
		return (1); /* continue the shell loop */
	}
	execute_command(args, prog_name);
	free_args(args);
	free(line);
	return (1); /* continue the shell loop */
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
		if (!handle_command(args, line, argv[0]))
			exit(0);
	}
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

