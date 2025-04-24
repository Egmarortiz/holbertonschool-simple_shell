#include "shell.h"

/**
 * handle_pipe_mode - Handle shell in non-interactive (pipe) mode
 * @state: Pointer to the shell state structure
 *
 * Return: Status code
 */
int handle_pipe_mode(shell_state_t *state)
{
	char *line;
	int status = 1;

	fprintf(stderr, "DEBUG: Processing commands from pipe\n");

	while (status)
	{
		/* Read command from stdin (pipe) */
		line = read_line();
		if (line == NULL)
		{
			fprintf(stderr, "DEBUG: End of piped input\n");
			break; /* EOF from pipe */
		}

		fprintf(stderr, "DEBUG: Processing piped command: '%s'\n", line);

		/* Process the command */
		status = process_command(line, state);

		/* Free the command line */
		free(line);
	}

	return (status);
}

/**
 * handle_interactive_mode - Handle shell in interactive mode
 * @state: Pointer to the shell state structure
 *
 * Return: Status code
 */
int handle_interactive_mode(shell_state_t *state)
{
	char *line;
	int status = 1;

	while (status)
	{
		/* Display prompt */
		display_prompt();

		/* Read command from user */
		line = read_line();
		if (line == NULL)
		{
			write(STDOUT_FILENO, "\n", 1); /* Print newline on EOF (Ctrl+D) */
			break;
		}

		/* Process the command */
		status = process_command(line, state);

		/* Free the command line */
		free(line);
	}

	return (status);
}

