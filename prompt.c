#include "shell.h"
#include <unistd.h>

/**
 * print_prompt - writes the shell prompt if running interactively.
 */
void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "($) ", 4);
}

