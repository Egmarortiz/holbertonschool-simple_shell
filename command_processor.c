#include "shell.h"

/**
 * handle_empty_command - Handle an empty command
 * @args: The parsed command arguments
 *
 * Return: Status code (1 to continue shell loop)
 */
int handle_empty_command(char **args)
{
	fprintf(stderr, "DEBUG: Empty command line, skipping execution\n");
	free_args(args);
	return (1);
}

/**
 * execute_parsed_command - Execute a parsed command
 * @args: The parsed command arguments
 * @state: Pointer to the shell state structure
 *
 * Return: Status code (1 to continue, 0 to exit)
 */
int execute_parsed_command(char **args, shell_state_t *state)
{
	int status = 1;
	int builtin_status;

	/* Check for built-in commands first */
	builtin_status = check_for_builtin(args, state);
	fprintf(stderr, "DEBUG: check_for_builtin returned: %d\n", builtin_status);

	if (builtin_status == 1) /* Not a builtin command */
	{
		/* If not a builtin, execute it as an external command */
		fprintf(stderr, "DEBUG: Executing as external command: '%s'\n", args[0]);
		status = execute_command(args, state);
	}
	else if (builtin_status == 0) /* Builtin executed successfully */
	{
		fprintf(stderr, "DEBUG: Executed as builtin command: '%s'\n", args[0]);
		status = 1; /* Continue the shell loop */
	}
	else /* Error in builtin */
	{
		fprintf(stderr, "DEBUG: Error in builtin command: '%s'\n", args[0]);
		status = 1; /* Continue despite error */
	}

	return (status);
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
	int i;

	/* Debug output to show the raw command line */
	fprintf(stderr, "DEBUG: Raw command line: '%s'\n", line);

	/* Parse the command line */
	args = split_line(line);
	if (args == NULL)
	{
		fprintf(stderr, "DEBUG: Command parsing failed\n");
		return (1);
	}

	/* Debug output to show parsed arguments */
	fprintf(stderr, "DEBUG: Parsed arguments:\n");
	for (i = 0; args[i] != NULL; i++)
	{
		fprintf(stderr, "DEBUG:   args[%d]: '%s'\n", i, args[i]);
	}

	/* If there were no arguments (empty line), skip execution */
	if (args[0] == NULL)
	{
		return (handle_empty_command(args));
	}

	status = execute_parsed_command(args, state);

	free_args(args);
	return (status);
}

