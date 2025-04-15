#include "shell.h"

/**
 * execute_command - Execute the command based on parsed arguments
 * @args: Array of arguments
 * @state: Pointer to shell state structure
 *
 * Return: 1 to continue the shell loop, 0 to exit
 */
int execute_command(char **args, shell_state_t *state)
{
	int builtin_status;

	if (args[0] == NULL)
		return (1);

	/* Check for built-in commands first */
	builtin_status = check_for_builtin(args, state);

	/* Handle different builtin execution results */
	if (builtin_status == 0)  /* Normal exit */
		return (0);
	if (builtin_status == 2)  /* Error in builtin */
		return (1);

	/* Only reach here if not a builtin command */
	return (launch_process(args, state));
}

/**
 * launch_external_process - Launch external process with given arguments
 * @args: Array of arguments
 * @state: Shell state
 *
 * Return: 1 to continue the shell loop
 */
int launch_external_process(char **args, shell_state_t *state)
{
	return (launch_process(args, state));
}

