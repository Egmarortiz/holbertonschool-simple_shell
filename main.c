#include "shell.h"

/**
 * init_shell_state - Initialize the shell state structure
 *
 * Return: An initialized shell_state_t structure
 */
shell_state_t init_shell_state(void)
{
	shell_state_t state;

	state.exit_status = 0;
	return (state);
}

/**
 * process_command - Process a single command line
 * @line: The command line to process
 * @state: Pointer to the shell state structure
 *
 * Return: 1 to continue, 0 to exit
 */
int process_command(char *line, shell_state_t *state)
{
	char **args;
	int status = 1;

	/* Parse the command line */
	args = split_line(line);
	if (args == NULL)
		return (1);

	/* If there were no arguments (empty line), skip execution */
	if (args[0] == NULL)
	{
		free_args(args);
		return (1);
	}

	/* Check for built-in commands first */
	if (check_for_builtin(args, state) == 0)
	{
		/* If not a builtin, execute it as an external command */
		status = execute_command(args, state);
	}

	free_args(args);
	return (status);
}

/**
 * shell_loop - Main loop for the shell
 * @state: Pointer to the shell state structure
 *
 * Return: The final exit status
 */
int shell_loop(shell_state_t *state)
{
	char *line;
	int status = 1;

	while (status)
	{
		/* Display prompt if in interactive mode */
		if (is_interactive())
			display_prompt();

		/* Read command from user */
		line = read_line();
		if (line == NULL)
		{
			if (is_interactive())
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Process the command */
		status = process_command(line, state);

		/* Free the command line */
		free(line);
	}

	return (state->exit_status);
}

/**
 * main - Entry point for the shell program
 * @argc: Number of command-line arguments
 * @argv: Array of command-line argument strings
 *
 * Return: Exit status of the shell
 */
int main(int argc, char **argv)
{
	shell_state_t state;
	int exit_status;

	(void)argc;
	(void)argv;

	/* Set up signal handlers */
	signal(SIGINT, handle_signal);

	/* Initialize shell state */
	state = init_shell_state();

	/* Run the shell loop */
	exit_status = shell_loop(&state);

	return (exit_status);
}

