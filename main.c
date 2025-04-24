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
 * shell_loop - Main loop for the shell
 * @state: Pointer to the shell state structure
 *
 * Return: The final exit status
 */
int shell_loop(shell_state_t *state)
{
	int is_pipe = !is_interactive();
	const char *mode = is_pipe ? "non-interactive (pipe)" : "interactive";

	fprintf(stderr, "DEBUG: Shell mode: %s\n", mode);

	if (is_pipe)
	{
		handle_pipe_mode(state);
	}
	else
	{
		handle_interactive_mode(state);
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

	/* Set appropriate buffering modes based on input type */
	if (!is_interactive())
	{
		/* Non-interactive mode (pipe) - disable buffering on all streams */
		setvbuf(stdin, NULL, _IONBF, 0);
		setvbuf(stdout, NULL, _IONBF, 0);
		setvbuf(stderr, NULL, _IONBF, 0);
	}

	/* Initialize shell state */
	state = init_shell_state();

	/* Run the shell loop */
	exit_status = shell_loop(&state);

	return (exit_status);
}
