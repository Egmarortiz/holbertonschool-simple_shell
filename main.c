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
		fprintf(stderr, "DEBUG: Empty command line, skipping execution\n");
		free_args(args);
		return (1);
	}

	/* Check for built-in commands first */
	int builtin_status = check_for_builtin(args, state);
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

	free_args(args);
	return (status);
}
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
	int is_pipe = !is_interactive();

	fprintf(stderr, "DEBUG: Shell mode: %s\n", is_pipe ? "non-interactive (pipe)" : "interactive");

	if (is_pipe)
	{
		/* In non-interactive mode, process commands without prompt */
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
	}
	else
	{
		/* Interactive mode with prompt */
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

