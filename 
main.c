#include "shell.h"

/**
 * main - Entry point for the shell
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: Exit status
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
