#include "shell.h"

/**
 * init_shell_state - Initialize shell state
 *
 * Return: Initialized shell state
 */
shell_state_t init_shell_state(void)
{
	shell_state_t state;

	state.exit_status = 0;
	return (state);
}

/**
 * main - Entry point for simple shell
 * @argc: Number of command-line arguments
 * @argv: Array of command-line argument strings
 *
 * Return: Exit status
 */
int main(int argc, char **argv)
{
	shell_state_t state;
	(void)argc;
	(void)argv;

	/* Initialize shell state */
	state = init_shell_state();

	/* Set up signal handling */
	signal(SIGINT, handle_signal);

	/* Run the shell loop */
	shell_loop(&state);

	return (state.exit_status);
}

/**
 * shell_loop - Main shell loop
 * @state: Pointer to shell state structure
 *
 * Return: 0 when shell exits
 */
int shell_loop(shell_state_t *state)
{
	char *line = NULL;
	int status = 1;

	while (status)
	{
		/* Display prompt if in interactive mode */
		if (is_interactive())
			display_prompt();

		/* Read command line */
		line = read_line();
		if (line == NULL) /* EOF (Ctrl+D) condition */
			break;

		/* Process the command */
		status = process_command(line, state);

		/* Clean up */
		free(line);
	}

	return (0);
}

/**
 * process_command - Process a command line
 * @line: Command line to process
 * @state: Pointer to shell state structure
 *
 * Return: 1 to continue, 0 to exit shell
 */
int process_command(char *line, shell_state_t *state)
{
	char **args = NULL;
	int status = 1;

	/* Skip empty lines */
	if (line[0] == '\0' || line[0] == '\n')
		return (1);

	/* Parse the line into tokens */
	args = split_line(line);
	if (args == NULL)
		return (1);

	/* Execute the command */
	status = execute_command(args, state);

	/* Clean up */
	free_args(args);

	return (status);
}
