#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * main - entry point for simple_shell with args & exit built-in
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
		if (args)
		{
			if (strcmp(args[0], "exit") == 0)
			{
				free_args(args);
				free(line);
				exit(0);
			}
			execute_command(args, argv[0]);
			free_args(args);
		}
		free(line);
	}
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

