#include "shell.h"

/**
 * is_interactive - Check if shell is running in interactive mode
 *
 * Return: 1 if interactive, 0 otherwise
 */
int is_interactive(void)
{
	return (isatty(STDIN_FILENO));
}

/**
 * handle_signal - Handle signals (e.g., Ctrl+C)
 * @sig: Signal number
 */
void handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n$ ", 3);
		fflush(stdout);
	}
}

/**
 * display_prompt - Display shell prompt
 */
void display_prompt(void)
{
	printf("$ ");
	fflush(stdout);
}

/**
 * read_line - Read a line from stdin
 *
 * Return: Pointer to line read (must be freed by caller)
 */
char *read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t chars_read;

	chars_read = getline(&line, &bufsize, stdin);

	if (chars_read == -1)
	{
		/* EOF or error */
		free(line);
		return (NULL);
	}

	/* Remove trailing newline if present */
	if (chars_read > 0 && line[chars_read - 1] == '\n')
		line[chars_read - 1] = '\0';

	return (line);
}

