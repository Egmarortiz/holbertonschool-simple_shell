#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * main - entry point for simple_shell.
 * @argc: argument count (unused).
 * @argv: argument vector, argv[0] used in errors.
 * Return: 0 on normal exit.
 */
int main(int argc, char **argv)
{
	char *line;

	(void)argc;
	while (1)
	{
		print_prompt();
		line = read_line();
		if (line == NULL) /* EOF or error */
			break;
		trim_newline(line);
		if (!is_blank(line))
			execute_command(line, argv[0]);
		free(line);
	}
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

