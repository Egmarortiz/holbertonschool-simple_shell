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
 * @should_exit: pointer to int indicating if shell should exit
 * Return: exit status of the command
 */
int handle_command(char **args, char *line, const char *prog_name,
		   int *should_exit)
{
	int exit_status = 0;

	if (strcmp(args[0], "exit") == 0)
	{
		free_args(args);
		free(line);
		*should_exit = 1;
		return (0); /* exit the shell with status 0 */
	}
	if (strcmp(args[0], "env") == 0)
	{
		print_environment();
		free_args(args);
		free(line);
		return (0); /* success status */
	}
	execute_command(args, prog_name, &exit_status);
	free_args(args);
	free(line);
	return (exit_status); /* return exit status from command */
}

/**
 * main - entry point for simple_shell with built-ins exit and env
 * @argc: arg count (unused)
 * @argv: arg vector; argv[0] used for errors
 * Return: exit status
 */
int main(int argc, char **argv)
{
	char *line;
	char **args;
	int exit_status = 0;
	int interactive;
	int should_exit = 0;
	(void)argc;

	interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
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

		exit_status = handle_command(args, line, argv[0], &should_exit);
		/* Exit if requested by a builtin or if in non-interactive mode */
		if (should_exit || !interactive)
			exit(exit_status);
	}

	if (interactive)
		write(STDOUT_FILENO, "\n", 1);
	return (exit_status);
}
